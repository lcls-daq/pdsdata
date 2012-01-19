#include "pdsdata/app/XtcMonitorServer.hh"

#include "pdsdata/xtc/ProcInfo.hh"
#include "pdsdata/xtc/Dgram.hh"
#include "pdsdata/ana/XtcRun.hh"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <queue>
#include <dirent.h>

using namespace Pds;
using namespace std;

//#define CLOCK CLOCK_PROCESS_CPUTIME_ID
#define CLOCK CLOCK_REALTIME

void sigsegv(int sig_no) {
}

//  (void) signal(SIGINT, sigfunc);

class MyMonitorServer : public XtcMonitorServer {
public:
  MyMonitorServer(const char* tag,
                  unsigned sizeofBuffers, 
                  unsigned numberofEvBuffers, 
                  unsigned numberofClients,
                  unsigned sequenceLength) :
    XtcMonitorServer(tag,
                     sizeofBuffers,
                     numberofEvBuffers,
                     numberofClients,
                     sequenceLength) 
  {
    //  sum of client queues (nEvBuffers) + clients + transitions + shuffleQ
    unsigned depth = 2*numberofEvBuffers+XtcMonitorServer::numberofTrBuffers+numberofClients;
    for(unsigned i=0; i<depth; i++)
      _pool.push(reinterpret_cast<Dgram*>(new char[sizeofBuffers]));
  }
  ~MyMonitorServer() 
  {
    while(!_pool.empty()) {
      delete _pool.front();
      _pool.pop();
    }
  }
public:
  XtcMonitorServer::Result events(Dgram* dg) {

    if (dg == NULL) {
      cerr << "dg is null!!!!" << endl;
      exit(1);
    }
    Xtc xtc = dg->xtc;
    if (XtcMonitorServer::events(dg) == XtcMonitorServer::Handled)
      _deleteDatagram(dg);
    return XtcMonitorServer::Deferred;
  }
  Dgram* newDatagram() 
  { 
    Dgram* dg = _pool.front(); 
    _pool.pop(); 
    return dg; 
  }
  void   deleteDatagram(Dgram* dg) { _deleteDatagram(dg); }
private:
  void  _deleteDatagram(Dgram* dg)
  {
    _pool.push(dg); 
  }
private:
  std::queue<Dgram*> _pool;
};

static MyMonitorServer* apps;

class XtcRunSet {
private:
  list<string> _paths;
  Ana::XtcRun _run;
  bool _runValid;

  // Internal method called when we have come to the
  // end of one run, and need to read more files to
  // start the next run. Returns true if we are
  // out of files.
  bool _skipToNextRun() {
    if (_paths.empty()) {
      return false;
    }
    string path = _paths.front();
    _run.reset(path);
    cout << endl << "Now processing " << path << endl;
    _paths.pop_front();
    while (! _paths.empty() && _run.add_file(_paths.front())) {
      _paths.pop_front();
    }
    _run.init();
    _runValid = true;
    return true;
  }

  // Takes a new list of paths, sorts it, and then appends the contents
  // to the existing list of paths (by moving the new paths).
  void _addPaths(list<string> newPaths) {
    newPaths.sort();
    _paths.splice(_paths.end(), newPaths);
  }

public:
  // Constructor that starts with an empty list of paths.
  XtcRunSet() : _runValid(false) {
  }

  // Fetch the next
  Dgram* next() {
    for (;;) {
      if (! _runValid && ! _skipToNextRun()) {
        return NULL;
      }
      Dgram* dg = NULL;
      int iSlice = -1;
      int64_t i64Offset = -1;
      Ana::Result result = _run.next(dg, &iSlice, &i64Offset);
      if (result == Ana::Error) {
        _runValid = false;
        continue; // need to skip to next run
      }
      return dg;
    }
  }

  void addSinglePath(string path) {
    _paths.push_back(path);
  }

  // Add every file in a directory whose full path contains the match string.
  void addPathsFromDir(string dirPath, string matchString) {
    DIR *dp = opendir(dirPath.c_str());
    if (dp == NULL) {
      string error = string("addPathsFromDir(") + dirPath + ")";
      perror(error.c_str());
      return;
    }
    list<string> newPaths;
    struct dirent *dirp;
    while ((dirp = readdir(dp)) != NULL) {
      if (strstr(dirp->d_name, ".xtc")) {
        string path = dirPath + "/" + dirp->d_name;
        cout << "~~~ looking at " << path << endl;
        if (path.find(matchString) != string::npos) {
          newPaths.push_back(path);
        }
      }
    }
    _addPaths(newPaths);
  }

  // Add every file containing the given run prefix.
  // The run prefix contains the full path of the directory to search.
  void addPathsFromRunPrefix(string runPrefix) {
    // Find *last* occurrence of -r in run prefix
    // (ignore earlier -r strings that could be in the directory path)
    size_t index = runPrefix.rfind("-r");
    if (index == string::npos) {
      cerr << runPrefix << " is not a valid run prefix." << endl;
      exit(1);
    }
    // The path will be of the form /.../.../eNNN-rNNNN-sNN-cNN.xtc,
    // so walk back 5 characters from the -r to capture
    // just the directory path.
    cout << "------> runPrefix = [" << runPrefix << "]" << endl;
    string dirPath = runPrefix.substr(0, index - 5);
    cout << "------> dirPath = [" << dirPath << "]" << endl;
    addPathsFromDir(dirPath, runPrefix);
  }
};

static void printTransition(const Dgram* dg) {
  printf("%18s transition: time %08x/%08x, payloadSize 0x%08x dmg 0x%x\n",
         TransitionId::name(dg->seq.service()),
         dg->seq.stamp().fiducials(),
         dg->seq.stamp().ticks(),
         dg->xtc.sizeofPayload(),
         dg->xtc.damage.value());
}

// Insert a simulated transition
static Dgram* insert(TransitionId::Value tr) {
  Dgram* dg = apps->newDatagram();
  new((void*)&dg->seq) Sequence(Sequence::Event, tr, ClockTime(0,0), TimeStamp(0,0,0,0));
  new((char*)&dg->xtc) Xtc(TypeId(TypeId::Id_Xtc,0),ProcInfo(Level::Event,0,0));
  printTransition(dg);
  return dg;
}
      
timespec& now() {
  static timespec now;
  clock_gettime(CLOCK, &now);
  return now;
}

long long int timeDiff(struct timespec* end, struct timespec* start) {
  long long int diff;
  diff =  (end->tv_sec - start->tv_sec) * 1000000000;
  diff += end->tv_nsec;
  diff -= start->tv_nsec;
  return diff;
}

void usage(char* progname) {
  cerr << "Usage: " << progname
       << " (-f <filename> | -l <filename_list> | -r <run_file_prefix> | -d <xtc_dir>)" // choose one
       << " -p <partitionTag> -n <numberOfBuffers> -s <sizeOfBuffers>" // mandatory
       << " [-z <ratePerSec>] [-c <# clients>] [-S <sequence length>]" // optional
       << " [-L] [-v] [-V]" // debugging (optional)
       << endl;
}

void sigfunc(int sig_no) {
  cout << endl;
  cout << endl;
  cout << endl;
  cout << endl;
  cout << endl;
  cout << "sigfunc!" << endl;
  //delete apps;
  exit(EXIT_SUCCESS);
}

void initRunSet(XtcRunSet& runSet, char *xtcname, char *filelist, char *runPrefix, char *xtcdir) {
  if (xtcname) {
    runSet.addSinglePath(xtcname);
  } else if (filelist) {
    // XXX runSet.addPathsFromFileList(filelist);
    cerr << "you lose" << endl;
    exit(1);
  } else if (xtcdir) {
    runSet.addPathsFromDir(xtcdir, xtcdir);
  } else {
    runSet.addPathsFromRunPrefix(runPrefix);
  }
}

int main(int argc, char* argv[]) {
  // Exactly one of these three must be supplied
  char* xtcname = NULL;
  char* filelist = NULL;
  char* runPrefix = NULL;
  char* xtcdir = NULL;

  // These are mandatory
  int numberOfBuffers = 0;
  unsigned sizeOfBuffers = 0;
  char* partitionTag = NULL;

  // These are optional
  int rate = 1; // Hz
  unsigned nclients = 1;
  unsigned sequenceLength = 1;

  // These are for debugging (also optional)
  bool loop = false;
  bool verbose = false;
  bool veryverbose = false;

  struct timespec start, now, sleepTime;
  (void) signal(SIGINT, sigfunc);

  int c;
  while ((c = getopt(argc, argv, "f:l:r:d:p:n:s:z:c:S:LvVh?")) != -1) {
    switch (c) {
      case 'f':
        xtcname = optarg;
        break;
      case 'l':
        filelist = optarg;
        break;
      case 'r':
        runPrefix = optarg;
        break;
      case 'd':
        xtcdir = optarg;
        break;
      case 'n':
        sscanf(optarg, "%d", &numberOfBuffers);
        break;
      case 's':
        sizeOfBuffers = (unsigned) strtoul(optarg, NULL, 0);
        break;
      case 'z':
        sscanf(optarg, "%d", &rate);
        break;
      case 'p':
        partitionTag = optarg;
        break;
      case 'c':
        nclients = strtoul(optarg, NULL, 0);
        break;
      case 'S':
        sscanf(optarg, "%d", &sequenceLength);
        break;
      case 'L':
        loop = true;
        printf("Enabling infinite looping\n");
        break;
      case 'v':
        verbose = true;
        break;
      case 'V':
        verbose = true;
        veryverbose = true;
        break;
      case 'h':
      case '?':
        usage(argv[0]);
        exit(0);
      default:
        fprintf(stderr, "Unrecognized option -%c!\n", c);
        usage(argv[0]);
        exit(0);
    }
  }

  if (sizeOfBuffers == 0 || numberOfBuffers == 0) {
    cerr << "Must specify both size (-s) and number (-n) of buffers." << endl;
    usage(argv[0]);
    exit(2);
  }
  if (partitionTag == NULL) {
    cerr << "Must specify a partition tag." << endl;
    usage(argv[0]);
    exit(2);
  }
  int choiceCount = (xtcname != NULL) + (filelist != NULL) + (runPrefix != NULL) + (xtcdir != NULL);
  if (choiceCount != 1) {
    cerr << "Must specify exactly one of -f, -l, -r, -d. You specified " << choiceCount << endl;
    usage(argv[0]);
    exit(2);
  }

  XtcRunSet runSet;
  initRunSet(runSet, xtcname, filelist, runPrefix, xtcdir);

  long long int period = 1000000000 / rate; // period in nanoseconds
  sleepTime.tv_sec = 0;
  long long int busyTime = period;
  cout << "Rate is " << rate << " Hz; period is " << period / 1e6 << " msec" << endl;

  clock_gettime(CLOCK, &start);

  apps = new MyMonitorServer(partitionTag,
                             sizeOfBuffers, 
                             numberOfBuffers, 
                             nclients,
                             sequenceLength);
  
  clock_gettime(CLOCK, &now);
  printf("Opening shared memory took %.3f msec.\n", timeDiff(&now, &start) / 1e6);

  do {
    apps->events(insert(TransitionId::Map));

    Dgram* dg;
    timespec loopStart;
    clock_gettime(CLOCK, &loopStart);
    int dgCount = 0;

    while ((dg = runSet.next()) != NULL) {
      timespec dgStart;
      clock_gettime(CLOCK, &dgStart);
      dgCount++;
      apps->events(dg);
      //      apps->routine();
      if (dg->seq.service() != TransitionId::L1Accept) {
        printTransition(dg);
        clock_gettime(CLOCK, &loopStart);
        dgCount = 0;
      } else if (verbose) {
        clock_gettime(CLOCK, &now);
        double hz = dgCount / (timeDiff(&now, &loopStart) / 1.e9);
        printf("%18s transition: time %08x/%08x, payloadSize 0x%08x, avg rate %8.3f Hz%c",
               TransitionId::name(dg->seq.service()),
               dg->seq.stamp().fiducials(),dg->seq.stamp().ticks(),
               dg->xtc.sizeofPayload(), hz,
               veryverbose ? '\n' : '\r');
      }

      clock_gettime(CLOCK, &now);
      busyTime = timeDiff(&now, &dgStart);
      if (period > busyTime) {
        sleepTime.tv_nsec = period - busyTime;
        if (nanosleep(&sleepTime, &now)<0) {
          perror("nanosleep");
        }
      }
    }

    apps->events(insert(TransitionId::Unconfigure));
    apps->events(insert(TransitionId::Unmap));

    if (loop) {
      // reset the runSet
      initRunSet(runSet, xtcname, filelist, runPrefix, xtcdir);
    }
  } while (loop);

  // XXX close run?
  sigfunc(0);

  return 0;
}
