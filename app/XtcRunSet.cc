#include "XtcRunSet.hh"

#include "pdsdata/app/XtcMonitorServer.hh"
#include "pdsdata/xtc/ProcInfo.hh"
#include <iostream>
#include <dirent.h>
#include <string.h>
#include <semaphore.h>

using namespace Pds;
using namespace std;

//#define CLOCK CLOCK_PROCESS_CPUTIME_ID
#define CLOCK CLOCK_REALTIME

static void printTransition(const Dgram* dg) {
  printf("%18s transition: time %016lx, payloadSize 0x%08x dmg 0x%x\n",
         TransitionId::name(dg->seq.service()),
         dg->seq.stamp().fiducials(),
         dg->xtc.sizeofPayload(),
         dg->xtc.damage.value());
}

static long long int itimeDiff(const timespec& end, const timespec& start)
{
  long long int diff;
  diff =  end.tv_sec - start.tv_sec;
  diff *= 1000000000;
  diff += end  .tv_nsec;
  diff -= start.tv_nsec;
  return diff;
}

static const unsigned numberofTrBuffers = 8;

class MyMonitorServer : public XtcMonitorServer {
private:
  queue<char*>  _evpool;
  queue<char*>  _trpool;
  queue<char*>  _simpool;
  sem_t         _sem;

  void _deleteDatagram(Dgram* dg) {
    if (dg->seq.service()==TransitionId::L1Accept)
      _evpool.push((char*)dg);
    else {
      _trpool.push((char*)dg);
    }
    sem_post(&_sem);
  }

public:
  MyMonitorServer(const char* tag,
                  unsigned sizeofBuffers, 
                  unsigned numberofEvBuffers, 
                  unsigned numberofClients) :
    XtcMonitorServer(tag,
                     sizeofBuffers*(numberofEvBuffers+numberofTrBuffers),
                     numberofEvBuffers,
                     numberofClients) {

    sem_init(&_sem,0,1);

    char* shm = _init();
    for(unsigned i=0; i<numberofEvBuffers; i++) {
      _evpool.push(shm);
      shm += sizeofBuffers;
    }
    for(unsigned i=0; i<numberofTrBuffers; i++) {
      _trpool.push(shm);
      shm += sizeofBuffers;
    }
    for(unsigned i=0; i<4; i++)
      _simpool.push(new char[sizeofBuffers]);
  }

  ~MyMonitorServer() {
    while(!_simpool.empty()) {
      delete _simpool.front();
      _simpool.pop();
    }
  }

  void events(Dgram* dg) {
    queue<char*>& q = (dg->seq.service()==TransitionId::L1Accept) ? _evpool : _trpool;
    while(q.empty())
      sem_wait(&_sem);
    char* p = q.front();
    q.pop();
    memcpy(p, dg, sizeof(*dg)+dg->xtc.sizeofPayload());
    XtcMonitorServer::events((Dgram*)p);
  }

  // Insert a simulated transition
  void insert(TransitionId::Value tr) {
    Dgram* dg = (Dgram*)_simpool.front(); 
    _simpool.pop(); 
    new((void*)&dg->seq) Sequence(Sequence::Event, tr, ClockTime(0,0), TimeStamp(0,0));
    new((char*)&dg->xtc) Xtc(TypeId(TypeId::Id_Xtc,0),ProcInfo(Level::Event,0,0));
    ::printTransition(dg);
    events(dg);
    _simpool.push((char*)dg);
  }
};

// Internal method called when we have come to the
// end of one run, and need to read more files to
// start the next run. Returns true if we are
// out of files.
bool XtcRunSet::_skipToNextRun() {
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
  _runIsValid = true;
  return true;
}

// Takes a new list of paths, sorts it, and then appends the contents
// to the existing list of paths (by moving the new paths).
void XtcRunSet::_addPaths(list<string> newPaths) {
  newPaths.sort();
  _paths.splice(_paths.end(), newPaths);
}

double XtcRunSet::timeDiff(struct timespec* end, struct timespec* start) {
  return double(itimeDiff(*end,*start));
}

// Constructor that starts with an empty list of paths.
XtcRunSet::XtcRunSet() :
  _runIsValid(false),
  _server(NULL) {
}

XtcRunSet::~XtcRunSet()
{
  if (_server) delete _server;
}

// Fetch the next
Dgram* XtcRunSet::next() {
  for (;;) {
    if (! _runIsValid && ! _skipToNextRun()) {
      return NULL;
    }
    Dgram* dg = NULL;
    int iSlice = -1;
    int64_t i64Offset = -1;
    Ana::Result result = _run.next(dg, &iSlice, &i64Offset);
    if (result != Ana::OK) {
      _runIsValid = false;
      continue; // need to skip to next run
    }
    return dg;
  }
}

void XtcRunSet::addSinglePath(string path) {
  _paths.push_back(path);
}

// Add every file in a directory whose full path contains the match string.
void XtcRunSet::addPathsFromDir(string dirPath, string matchString) {
  cout << "addPathsFromDir(dirPath=[" << dirPath << "], matchstring=[" << matchString << "])" << endl;
  DIR *dp = opendir((dirPath == "") ? "." : dirPath.c_str());
  if (dp == NULL) {
    string error = string("addPathsFromDir(") + dirPath + ")";
    perror(error.c_str());
    return;
  }
  list<string> newPaths;
  struct dirent *dirp;
  while ((dirp = readdir(dp)) != NULL) {
    if (strstr(dirp->d_name, ".xtc")) {
      cout << "dirp->d_name = [" << dirp->d_name << "]" << endl;
      string path = (dirPath == "" ? dirp->d_name : (dirPath + "/" + dirp->d_name));
      cout << "path = [" << path << "]" << endl;
      if (matchString.empty() || (path.find(matchString) != string::npos)) {
        newPaths.push_back(path);
      }
    }
  }
  _addPaths(newPaths);
}

// Add every file containing the given run prefix.
// The run prefix contains the full path of the directory to search.
void XtcRunSet::addPathsFromRunPrefix(string runPrefix) {
  size_t lastSlash = runPrefix.rfind("/");
  if (lastSlash == string::npos) {
    addPathsFromDir("", runPrefix);
  } else {
    string dirPath = runPrefix.substr(0, lastSlash);
    runPrefix = runPrefix.substr(lastSlash + 1);
    addPathsFromDir(dirPath, runPrefix);
  }
}

// Add every file listed in the list file.
void XtcRunSet::addPathsFromListFile(string listFile) {
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

void XtcRunSet::connect(char* partitionTag, unsigned sizeOfBuffers, int numberOfBuffers, unsigned nclients, int rate, bool verbose, bool veryverbose) {
  if (_server == NULL) {
    _verbose = verbose;
    _veryverbose = veryverbose;

    if (rate > 0) {
      _period = 1000000000 / rate; // period in nanoseconds
      cout << "Rate is " << rate << " Hz; period is " << _period / 1e6 << " msec" << endl;
    } else {
      _period = 0;
      cout << "Rate was not specified; will run unthrottled." << endl;
    }

    struct timespec start, now;
    clock_gettime(CLOCK, &start);
    _server = new MyMonitorServer(partitionTag,
                                  sizeOfBuffers, 
                                  numberOfBuffers, 
                                  nclients);
    clock_gettime(CLOCK, &now);
    printf("Opening shared memory took %.3f msec.\n", timeDiff(&now, &start) / 1e6);
  }
}

void XtcRunSet::run() {
  _server->insert(TransitionId::Map);

  Dgram* dg;
  timespec loopStart;
  clock_gettime(CLOCK, &loopStart);
  int dgCount = 0;

  while ((dg = next()) != NULL) {
    timespec dgStart;
    clock_gettime(CLOCK, &dgStart);
    dgCount++;
    _server->events(dg);
    //      _server->routine();
    if (dg->seq.service() != TransitionId::L1Accept) {
      printTransition(dg);
      clock_gettime(CLOCK, &loopStart);
      dgCount = 0;
    } else if (_verbose) {
      timespec now;
      clock_gettime(CLOCK, &now);
      double hz = double(dgCount) / (timeDiff(&now, &loopStart) / 1.e9);
      printf("%18s transition: time %016lx, payloadSize 0x%08x, avg rate %8.3f Hz%c",
             TransitionId::name(dg->seq.service()),
             dg->seq.stamp().fiducials(),
             dg->xtc.sizeofPayload(), hz,
             _veryverbose ? '\n' : '\r');
    }

    if (_period != 0) {
      timespec now;
      clock_gettime(CLOCK, &now);
      long long int busyTime = itimeDiff(now, dgStart);
      if (_period > busyTime) {
        timespec sleepTime;
        sleepTime.tv_sec = 0;
        sleepTime.tv_nsec = _period - busyTime;
        if (nanosleep(&sleepTime, &now) < 0) {
          perror("nanosleep");
        }
      }
    }
  }
  //  XtcRun sinks EndRun
  //  _server->insert(TransitionId::EndRun);

  _server->insert(TransitionId::Unconfigure);
  _server->insert(TransitionId::Unmap);
}
