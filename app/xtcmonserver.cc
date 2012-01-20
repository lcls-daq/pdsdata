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

static void sigfunc(int sig_no) {
  cout << endl << "Caught " << strsignal(sig_no) << " signal, exiting..." << endl;
  exit(0);
}

static void printTransition(const Dgram* dg) {
  printf("%18s transition: time %08x/%08x, payloadSize 0x%08x dmg 0x%x\n",
         TransitionId::name(dg->seq.service()),
         dg->seq.stamp().fiducials(),
         dg->seq.stamp().ticks(),
         dg->xtc.sizeofPayload(),
         dg->xtc.damage.value());
}

class MyMonitorServer : public XtcMonitorServer {
private:
  queue<Dgram*> _pool;

  void _deleteDatagram(Dgram* dg) {
    _pool.push(dg); 
  }

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
                     sequenceLength) {
    //  sum of client queues (nEvBuffers) + clients + transitions + shuffleQ
    unsigned depth = 2*numberofEvBuffers+XtcMonitorServer::numberofTrBuffers+numberofClients;
    for(unsigned i=0; i<depth; i++)
      _pool.push(reinterpret_cast<Dgram*>(new char[sizeofBuffers]));
  }

  ~MyMonitorServer() {
    while(!_pool.empty()) {
      delete _pool.front();
      _pool.pop();
    }
  }

  XtcMonitorServer::Result events(Dgram* dg) {
    Xtc xtc = dg->xtc;
    if (XtcMonitorServer::events(dg) == XtcMonitorServer::Handled) {
      _deleteDatagram(dg);
    }
    return XtcMonitorServer::Deferred;
  }

  // Insert a simulated transition
  void insert(TransitionId::Value tr) {
    Dgram* dg = _pool.front(); 
    _pool.pop(); 
    new((void*)&dg->seq) Sequence(Sequence::Event, tr, ClockTime(0,0), TimeStamp(0,0,0,0));
    new((char*)&dg->xtc) Xtc(TypeId(TypeId::Id_Xtc,0),ProcInfo(Level::Event,0,0));
    ::printTransition(dg);
    events(dg);
  }
};

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
    cout << endl << "Adding files for new run..." << endl;
    _run.reset(_paths.front());
    cout << "Adding " << _paths.front() << endl;
    _paths.pop_front();
    while (! _paths.empty() && _run.add_file(_paths.front())) {
      cout << "Adding " << _paths.front() << endl;
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
      if (result != Ana::OK) {
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
  void addPathsFromDir(string dirPath, string matchString = "") {
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
        if (matchString.empty() || (path.find(matchString) != string::npos)) {
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
    string dirPath = runPrefix.substr(0, index - 5);
    addPathsFromDir(dirPath, runPrefix);
  }

  // Add every file listed in the list file.
  void addPathsFromListFile(string listFile) {
    cout << "addPathsFromListFile(" << listFile << ")" << endl;
    FILE *flist = fopen(listFile.c_str(), "r");
    if (flist == NULL) {
      perror(listFile.c_str());
      return;
    }
    list<string> newPaths;
    char filename[FILENAME_MAX];
    while (fscanf(flist, "%s", filename) != EOF) {
      newPaths.push_back(filename);
    }
    _addPaths(newPaths);
  }
};

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
       << " (-f <filename> | -l <filename_list> | -x <run_file_prefix> | -d <xtc_dir>)" // choose one
       << " -p <partitionTag> -n <numberOfBuffers> -s <sizeOfBuffers>" // mandatory
       << " [-r <ratePerSec>] [-c <# clients>] [-S <sequence length>]" // optional
       << " [-L] [-v] [-V]" // debugging (optional)
       << endl;
}

void initRunSet(XtcRunSet& runSet, char *xtcFile, char *xtcDir, char *listFile, char *runPrefix) {
  if (xtcFile) {
    runSet.addSinglePath(xtcFile);
  } else if (xtcDir) {
    runSet.addPathsFromDir(xtcDir);
  } else if (listFile) {
    runSet.addPathsFromListFile(listFile);
  } else {
    runSet.addPathsFromRunPrefix(runPrefix);
  }
}

int main(int argc, char* argv[]) {
  // Exactly one of these values must be supplied
  char* xtcFile = NULL;
  char* listFile = NULL;
  char* runPrefix = NULL;
  char* xtcDir = NULL;

  // These are mandatory
  int numberOfBuffers = 4;
  unsigned sizeOfBuffers = 0x900000;
  char* partitionTag = NULL;

  // These are optional
  int rate = 60; // Hz
  unsigned nclients = 1;
  unsigned sequenceLength = 1;

  // These are for debugging (also optional)
  bool loop = false;
  bool verbose = false;
  bool veryverbose = false;

  struct timespec start, now, sleepTime;
  (void) signal(SIGINT, sigfunc);
  (void) signal(SIGSEGV, sigfunc);

  int c;
  while ((c = getopt(argc, argv, "f:l:x:d:p:n:s:r:c:S:LvVh?")) != -1) {
    switch (c) {
      case 'f':
        xtcFile = optarg;
        break;
      case 'l':
        listFile = optarg;
        break;
      case 'x':
        runPrefix = optarg;
        break;
      case 'd':
        xtcDir = optarg;
        break;
      case 'n':
        sscanf(optarg, "%d", &numberOfBuffers);
        break;
      case 's':
        sizeOfBuffers = (unsigned) strtoul(optarg, NULL, 0);
        break;
      case 'r':
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
    partitionTag = getenv("USER");
    if (partitionTag == NULL) {
      cerr << "Must specify a partition tag." << endl;
      usage(argv[0]);
      exit(2);
    }
  }
  int choiceCount = (xtcFile != NULL) + (listFile != NULL) + (runPrefix != NULL) + (xtcDir != NULL);
  if (choiceCount != 1) {
    cerr << "Must specify exactly one of -f, -l, -r, -d. You specified " << choiceCount << endl;
    usage(argv[0]);
    exit(2);
  }

  XtcRunSet runSet;
  initRunSet(runSet, xtcFile, xtcDir, listFile, runPrefix);

  long long int period = 0;
  long long int busyTime = period;
  if (rate > 0) {
    period = 1000000000 / rate; // period in nanoseconds
    sleepTime.tv_sec = 0;
    busyTime = period;
    cout << "Rate is " << rate << " Hz; period is " << period / 1e6 << " msec" << endl;
  } else {
    cout << "Rate was not specified; will run unthrottled." << endl;
  }

  clock_gettime(CLOCK, &start);

  MyMonitorServer server = MyMonitorServer(partitionTag,
                                           sizeOfBuffers, 
                                           numberOfBuffers, 
                                           nclients,
                                           sequenceLength);

  clock_gettime(CLOCK, &now);
  printf("Opening shared memory took %.3f msec.\n", timeDiff(&now, &start) / 1e6);

  do {
    server.insert(TransitionId::Map);

    Dgram* dg;
    timespec loopStart;
    clock_gettime(CLOCK, &loopStart);
    int dgCount = 0;

    while ((dg = runSet.next()) != NULL) {
      timespec dgStart;
      clock_gettime(CLOCK, &dgStart);
      dgCount++;
      server.events(dg);
      //      server.routine();
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

      if (rate > 0) {
        clock_gettime(CLOCK, &now);
        busyTime = timeDiff(&now, &dgStart);
        if (period > busyTime) {
          sleepTime.tv_nsec = period - busyTime;
          if (nanosleep(&sleepTime, &now) < 0) {
            perror("nanosleep");
          }
        }
      }
    }

    server.insert(TransitionId::Unconfigure);
    server.insert(TransitionId::Unmap);

    if (loop) {
      // reset the runSet
      initRunSet(runSet, xtcFile, xtcDir, listFile, runPrefix);
    }
  } while (loop);

  return 0;
}
