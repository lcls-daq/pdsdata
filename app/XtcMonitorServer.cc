#include "pdsdata/app/XtcMonitorServer.hh"

#include "pdsdata/xtc/Dgram.hh"

#ifdef _POSIX_MESSAGE_PASSING
#include <mqueue.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/prctl.h>

using std::queue;
using std::stack;

#define PERMS (S_IRUSR|S_IRGRP|S_IROTH|S_IWUSR|S_IWGRP|S_IWOTH)
#define OFLAGS (O_CREAT|O_RDWR)

namespace Pds {
  class ShMsg {
  public:
    ShMsg() {}
    ShMsg(const XtcMonitorMsg&  m,
	  Dgram* dg) : _m(m), _dg(dg) {}
    ~ShMsg() {}
  public:
    const XtcMonitorMsg&  msg() const { return _m; }
    Dgram* dg () const { return _dg; }
  private:
    XtcMonitorMsg _m;
    Dgram*        _dg;
  };

  class EventSequence {
  public:
    EventSequence(unsigned n) :
      _dgram  (new Dgram*[n]),
      _current(0),
      _depth  (n) {}
    ~EventSequence() { delete[] _dgram; }
  public:
    bool     complete()           const { return _current==_depth; }
    Dgram*   dgram   (unsigned i) const { return _dgram[i]; }
    unsigned current ()           const { return _current; }
    unsigned depth   ()           const { return _depth; }
  public:
    void insert(Dgram* dg) { _dgram[_current++] = dg; }
    void clear () { _current = 0; }
  private:
    Dgram**  _dgram;
    unsigned _current;
    unsigned _depth;
  };
};

using namespace Pds;

static const int numberofTrBuffers = 8;


XtcMonitorServer::XtcMonitorServer(unsigned sizeofBuffers, 
				   unsigned numberofEvBuffers,
				   unsigned numberofClients,
				   unsigned sequenceLength) :
  _sizeOfBuffers    (sizeofBuffers),
  _numberOfEvBuffers(numberofEvBuffers),
  _numberOfClients  (numberofClients),
  _sequence(new EventSequence(sequenceLength))
{
  _myMsg.numberOfBuffers(numberofEvBuffers+numberofTrBuffers);
  _myMsg.sizeOfBuffers  (sizeofBuffers);

  _tmo.tv_sec  = 0;
  _tmo.tv_nsec = 0;

  sem_init(&_sem, 0, 1);
}

XtcMonitorServer::~XtcMonitorServer() 
{ 
  delete _sequence;
  sem_destroy(&_sem);
  pthread_kill(_threadID, SIGTERM);
  printf("Not Unlinking Shared Memory... \n");
}

XtcMonitorServer::Result XtcMonitorServer::events(Dgram* dg) 
{
  Dgram& dgrm = *dg;

  if (dgrm.seq.service() == TransitionId::L1Accept) {
    if (!_sequence->complete()) {
      _sequence->insert(dg);
      return Deferred;
    }
    else {
      mq_getattr(_myInputEvQueue, &_mymq_attr);
      unsigned depth = _sequence->depth();
      if (_mymq_attr.mq_curmsgs >= int(depth)) {
	for(unsigned i=0; i<depth; i++) {
	  if (mq_receive(_myInputEvQueue, (char*)&_myMsg, sizeof(_myMsg), NULL) < 0) 
	    perror("mq_receive");

	  ShMsg m(_myMsg, _sequence->dgram(i));
	  if (mq_timedsend(_shuffleQueue, (const char*)&m, sizeof(m), 0, &_tmo)) {
	    printf("ShuffleQ timedout\n");
	    _deleteDatagram(_sequence->dgram(i));
	  }
	}
	_sequence->clear();
	_sequence->insert(dg);
	return Deferred;
      }
    }
  }
  else {

    for(unsigned i=0; i<_sequence->current(); i++)
      _deleteDatagram(_sequence->dgram(i));
    _sequence->clear();

    if (_freeTr.empty()) {
      printf("No buffers available for transition!\n");
      abort();
    }

    int ibuffer = _freeTr.front(); _freeTr.pop();

    _myMsg.bufferIndex(ibuffer);
    _copyDatagram(dg, _myShm + _sizeOfBuffers*ibuffer);

    sem_wait(&_sem);

    if (unsigned(dgrm.seq.service())%2) {
      _pop_transition();
      _freeTr.push(ibuffer);
    }
    else 
      _push_transition(ibuffer);

    sem_post(&_sem);

    for(unsigned i=0; i<_numberOfClients; i++) {
      if (mq_timedsend(_myOutputTrQueue[i], (const char*)&_myMsg, sizeof(_myMsg), 0, &_tmo))
        ;  // best effort
    }

    _moveQueue(_myOutputEvQueue, _myInputEvQueue);
  }
  return Handled;
}

void XtcMonitorServer::routine()
{
  while(1) {
    if (::poll(_pfd,2,-1) > 0) {
      if (_pfd[0].revents & POLLIN)
        _initialize_client();

      if (_pfd[1].revents & POLLIN) {
        ShMsg m;
        if (mq_receive(_shuffleQueue, (char*)&m, sizeof(m), NULL) < 0)
          perror("mq_receive");

        _copyDatagram(m.dg(),_myShm+_sizeOfBuffers*m.msg().bufferIndex());
        _deleteDatagram(m.dg());

        if (mq_timedsend(_myOutputEvQueue, (const char*)&m.msg(), sizeof(m.msg()), 0, &_tmo)) {
          printf("outputEv timedout\n");
        }
      }
    }
  }
}

static void* TaskRoutine(void* task)
    {
  XtcMonitorServer* srv = (XtcMonitorServer*)task;
  srv->routine();
  return srv;
    }

int XtcMonitorServer::init(char *p) 
{ 
  char* shmName    = new char[128];
  char* toQname    = new char[128];
  char* fromQname  = new char[128];

  sprintf(shmName  , "/PdsMonitorSharedMemory_%s",p);
  sprintf(toQname  , "/PdsToMonitorEvQueue_%s",p);
  sprintf(fromQname, "/PdsFromMonitorEvQueue_%s",p);
  _pageSize = (unsigned)sysconf(_SC_PAGESIZE);

  int ret = 0;
  _sizeOfShm = (_numberOfEvBuffers + numberofTrBuffers) * _sizeOfBuffers;
  unsigned remainder = _sizeOfShm%_pageSize;
  if (remainder) _sizeOfShm += _pageSize - remainder;

  _mymq_attr.mq_maxmsg  = _numberOfEvBuffers;
  _mymq_attr.mq_msgsize = (long int)sizeof(XtcMonitorMsg);
  _mymq_attr.mq_flags   = O_NONBLOCK;

  umask(1);  // try to enable world members to open these devices.

  int shm = shm_open(shmName, OFLAGS, PERMS);
  if (shm < 0) {ret++; perror("shm_open");}

  if ((ftruncate(shm, _sizeOfShm))<0) {ret++; perror("ftruncate");}

  _myShm = (char*)mmap(NULL, _sizeOfShm, PROT_READ|PROT_WRITE, MAP_SHARED, shm, 0);
  if (_myShm == MAP_FAILED) {ret++; perror("mmap");}

  _flushQueue(_myOutputEvQueue = _openQueue(toQname));

  _flushQueue(_myInputEvQueue  = _openQueue(fromQname));

  sprintf(fromQname, "/PdsFromMonitorDiscovery_%s",p);
  _pfd[0].fd      = _discoveryQueue  = _openQueue(fromQname);
  _pfd[0].events  = POLLIN;
  _pfd[0].revents = 0;

  _myOutputTrQueue = new mqd_t[_numberOfClients];
  for(unsigned i=0; i<_numberOfClients; i++) {
    sprintf(toQname  , "/PdsToMonitorTrQueue_%s_%d",p,i);
    _flushQueue(_myOutputTrQueue[i] = _openQueue(toQname));
  }

  struct mq_attr shq_attr;
  shq_attr.mq_maxmsg  = _numberOfEvBuffers;
  shq_attr.mq_msgsize = (long int)sizeof(ShMsg);
  shq_attr.mq_flags   = O_NONBLOCK;
  sprintf(toQname, "/PdsShuffleQueue_%s",p);
  _shuffleQueue = _openQueue(toQname, shq_attr);
  { ShMsg m; _flushQueue(_shuffleQueue,(char*)&m, sizeof(m)); }

  _pfd[1].fd = _shuffleQueue;
  _pfd[1].events  = POLLIN;
  _pfd[1].revents = 0;

  // create the listening thread
  pthread_create(&_threadID,NULL,TaskRoutine,this);

  // prestuff the input queue which doubles as the free list
  for (int i=0; i<_numberOfEvBuffers; i++) {
    if (mq_send(_myInputEvQueue, (const char *)_myMsg.bufferIndex(i),
        sizeof(XtcMonitorMsg), 0)) {
      perror("mq_send inQueueStuffing");
      delete this;
      exit(EXIT_FAILURE);
    }
  }

  for(int i=0; i<numberofTrBuffers; i++)
    _freeTr.push(i+_numberOfEvBuffers);

  delete[] shmName;
  delete[] toQname;
  delete[] fromQname;

  return ret;
}

void XtcMonitorServer::_initialize_client()
{
  sem_wait(&_sem);

  XtcMonitorMsg msg;
  if (mq_receive(_discoveryQueue, (char*)&msg, sizeof(msg), NULL) < 0) 
    perror("mq_receive");

  unsigned iclient = msg.bufferIndex();
  printf("_initialize_client %d\n",iclient);

  std::stack<int> tr;
  while(!_cachedTr.empty()) {
    tr.push(_cachedTr.top());
    _cachedTr.pop();
  }
  while(!tr.empty()) {
    int ibuffer = tr.top(); tr.pop();
    _myMsg.bufferIndex(ibuffer);

    //     { Dgram& dgrm = *reinterpret_cast<Dgram*>(_myShm + _sizeOfBuffers * _myMsg.bufferIndex());
    //       printf("Sending tr %s to mq %d\nmsg %x/%x/%x\n",
    // 	     TransitionId::name(dgrm.seq.service()),
    // 	     _myOutputTrQueue[iclient],
    // 	     _myMsg.bufferIndex(),
    // 	     _myMsg.numberOfBuffers(),
    // 	     _myMsg.sizeOfBuffers()); }

    if (mq_send(_myOutputTrQueue[iclient], (const char*)&_myMsg, sizeof(_myMsg), 0)) 
      ;   // best effort only
    _cachedTr.push(ibuffer);
  }

  sem_post(&_sem);
}

void XtcMonitorServer::_copyDatagram(Dgram* p, char* b) 
{
  Dgram* dg = (Dgram*)p;
  memcpy((char*)b, dg, sizeof(Dgram)+dg->xtc.sizeofPayload());
}

void XtcMonitorServer::_deleteDatagram(Dgram* p) {}

mqd_t XtcMonitorServer::_openQueue(const char* name)
{
  return _openQueue(name, _mymq_attr);
}

mqd_t XtcMonitorServer::_openQueue(const char* name, mq_attr& attr) 
{
  mqd_t q = mq_open(name,  O_CREAT|O_RDWR, PERMS, &attr);
  if (q == (mqd_t)-1) {
    perror("mq_open output");
    printf("mq_attr:\n\tmq_flags 0x%0lx\n\tmq_maxmsg 0x%0lx\n\tmq_msgsize 0x%0lx\n\t mq_curmsgs 0x%0lx\n",
        attr.mq_flags, attr.mq_maxmsg, attr.mq_msgsize, attr.mq_curmsgs );
    fprintf(stderr, "Initializing XTC monitor server encountered an error!\n");
    delete this;
    exit(EXIT_FAILURE);
  }
  else {
    printf("Opened queue %s (%d)\n",name,q);
  }
  return q;
}

void XtcMonitorServer::_flushQueue(mqd_t q)
{
  XtcMonitorMsg m; 
  _flushQueue(q,(char*)&m,sizeof(m)); 
}

void XtcMonitorServer::_flushQueue(mqd_t q, char* m, unsigned sz) 
{
  // flush the queues just to be sure they are empty.
  struct mq_attr attr;
  do {
    mq_getattr(q, &attr);
    if (attr.mq_curmsgs)
      mq_receive(q, m, sz, NULL);
  } while (attr.mq_curmsgs);
}

void XtcMonitorServer::_moveQueue(mqd_t iq, mqd_t oq) 
{
  XtcMonitorMsg m;
  struct mq_attr attr;
  do {
    mq_getattr(iq, &attr);
    if (attr.mq_curmsgs) {
      if (mq_receive(iq, (char*)&m, sizeof(m), NULL) == -1)
        perror("moveQueue: mq_receive");
      if (mq_send   (oq, (char*)&m, sizeof(m), 0) == -1) {
        printf("Failed to reclaim buffer %i : %s\n",
            m.bufferIndex(), strerror(errno));
      }
    }
  } while (attr.mq_curmsgs);
}

void XtcMonitorServer::_push_transition(int ibuffer)
{
  _cachedTr.push(ibuffer);
}

void XtcMonitorServer::_pop_transition()
{
  _freeTr.push(_cachedTr.top());
  _cachedTr.pop();
}

