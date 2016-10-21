#include "pdsdata/app/XtcMonitorServer.hh"

#include "pdsdata/app/TransitionCache.hh"
#include "pdsdata/xtc/Dgram.hh"

#include <unistd.h>
#include <semaphore.h>
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
#include <sys/stat.h>
#include <time.h>

#include <sys/socket.h>
#include <arpa/inet.h>

#include <list>

//#define DBUG
//#define NO_STEAL

using std::queue;
using std::stack;

//
//  Recover any shared memory buffer older than 10 seconds
//
static const unsigned TMO_SEC = 10;

#define PERMS (S_IRUSR|S_IRGRP|S_IROTH|S_IWUSR|S_IWGRP|S_IWOTH)
#define PERMS_IN (S_IRUSR|S_IRGRP|S_IROTH)
#define OFLAGS (O_CREAT|O_RDWR)

namespace Pds {
  class XtcMonitorServerF {
  public:
    XtcMonitorServerF(void* p) : _srv(*(XtcMonitorServer*)p) {}
    XtcMonitorServerF(XtcMonitorServer* srv) : _srv(*srv) {}
  public:
    void discover() { _srv._discover(); }
    void routine () { _srv._routine (); }
    void unlink  () { _srv._unlink  (); }
  private:
    XtcMonitorServer& _srv;
  };
};

static Pds::XtcMonitorServer* apps;

static struct sigaction old_actions[64];

void sigfunc(int sig_no) {
  static bool _handled=false;
  if (!_handled) {
    _handled = true;
    printf("handling signal %d app %p\n",sig_no,apps);
    if (apps) {
      Pds::XtcMonitorServerF(apps).unlink();
      apps = 0;
    }
    else
      printf("nothing to do\n");

    printf("done with signal %d\n",sig_no);
    
    sigaction(sig_no,&old_actions[sig_no],NULL);
    raise(sig_no);
  }
}

using namespace Pds;

XtcMonitorServer::XtcMonitorServer(const char* tag,
                                   uint64_t    sizeofMem,
				   unsigned    numberofEvBuffers,
				   unsigned    numberofEvQueues) :
  _tag              (tag),
  _sizeofMem        (sizeofMem),
  _numberOfEvBuffers(numberofEvBuffers),
  _numberOfEvQueues (numberofEvQueues),
  _myShm            (0),
  _discoveryQueue   (-1),
  _myInputEvQueue   (-1),
  _myOutputEvQueue  (new mqd_t[numberofEvQueues]),
  _myTrFd           (0),
  _msgDest          (numberofEvQueues),
  _pfd              (new pollfd[32]),
  _nfd              (2),
  _requestQueue     (-1),
  _ievt             (0)
{
  _myMsg.sizeOfMem      (sizeofMem);
  _myMsg.return_queue   (numberofEvQueues);

  _tmo.tv_sec  = 0;
  _tmo.tv_nsec = 0;

  apps = this;

  struct sigaction int_action;

  int_action.sa_handler = sigfunc;
  sigemptyset(&int_action.sa_mask);
  int_action.sa_flags = 0;
  int_action.sa_flags |= SA_RESTART;

#define REGISTER(t) {                                               \
    if (sigaction(t, &int_action, &old_actions[t]) > 0)             \
      printf("Couldn't set up #t handler\n");                       \
  }

  REGISTER(SIGINT);
  REGISTER(SIGSEGV);
  REGISTER(SIGABRT);
  REGISTER(SIGTERM);
  REGISTER(SIGPIPE);

#undef REGISTER
}

XtcMonitorServer::~XtcMonitorServer() 
{ 
  pthread_kill(_discThread, SIGTERM);
  pthread_kill(_taskThread  , SIGTERM);
  printf("Not Unlinking Shared Memory... \n");

  _unlink();

  delete _transitionCache;
}

bool XtcMonitorServer::_send(Dgram* dg)
{
  //
  //  For reasons I don't yet understand, sometimes the message queues
  //  are opened in blocking mode.  So, I use mq_timedreceive 
  //  with a 0 timeout to avoid blocking.
  //
  XtcMonitorMsg msg;
  const timespec no_wait={0,0};
  int r = mq_timedreceive(_requestQueue, (char*)&msg, sizeof(msg), NULL,
                          &no_wait); 

#ifndef NO_STEAL
  static unsigned _nsteals=0;
  if (r>0)
    ;
  else {
    if (r<0) ; // perror("Error reading input event queue");
    for(unsigned i=0; i<_numberOfEvQueues; i++) {
      unsigned iq = (i+_nsteals)%_numberOfEvQueues; // fairness
      r=mq_timedreceive(_myOutputEvQueue[iq], (char*)&msg, sizeof(msg), NULL,
                        &no_wait);
      if (r>0) break;
      if (r<0) ; // perror("Error reading output event queue");
    }
    _nsteals++;
  }
#endif

  if (r>0) {
    //
    //  Send this event to the next client around (round-robin)
    //
    bool lsent=false;
    for(unsigned i=0; i<_numberOfEvQueues; i++) {
      int oc = _ievt++%_numberOfEvQueues;
      int oq = _myOutputEvQueue[oc];
      msg.bufferOffset(reinterpret_cast<char*>(dg)-_myShm);
      if (mq_timedsend(oq, (const char*)&msg, sizeof(msg), 0, &_tmo))
        ;
      else {
        _msgDest[oc].push_back(msg.bufferOffset());
        lsent=true;
        break;
      }
    }
    if (!lsent) {
      perror("Unable to distribute or reclaim event");
    }
  }
  else {
    // No shared memory buffer found.  Dropping new event.
    _deleteDatagram(dg);
  }
  return true;
}

void XtcMonitorServer::events(Dgram* dg) 
{
  Dgram& dgrm = *dg;
  if (dgrm.seq.service() == TransitionId::L1Accept) {
    _send(dg);
  }
  else {

    TransitionId::Value trid = dgrm.seq.service();

    XtcMonitorMsg myMsg(_myMsg);
    myMsg.bufferOffset( reinterpret_cast<char*>(dg)-_myShm );

    _transitionCache->allocate(dg, trid);

    if (trid == TransitionId::Enable) {
      //
      //  Steal all event buffers from the clients
      //
      for(unsigned i=0; i<_numberOfEvQueues; i++)
        _moveQueue(_myOutputEvQueue[i], _myInputEvQueue, i);
    }

    //
    //  Broadcast the transition to all ready clients
    //
    for(unsigned i=0; i<_myTrFd.size(); i++) {
      int oq = _myTrFd[i];
      if (oq == -1 || !_transitionCache->allocate(dg,i))
	continue;
      if (::send(oq, (const char*)&myMsg, sizeof(myMsg), 0) < 0) {
	perror("Error sending transition");
	_transitionCache->deallocate(dg,i);
      }
    }

  }
}

void XtcMonitorServer::_discover()
{
  int fd;
  if ( (fd = ::socket(AF_INET, SOCK_STREAM, 0))<0 ) {
    perror("Create discovery socket failed");
    exit(1);
  }

  //  assign an ephemeral port
  unsigned port = 32768;
  sockaddr_in saddr;
  saddr.sin_family      = AF_INET;
  saddr.sin_addr.s_addr = htonl(0x7f000001);
  while(1) {
    saddr.sin_port        = htons(port);
#ifdef DBUG
    printf("Trying to bind to port %d\n",port);
#endif
    if (::bind(fd,(sockaddr*)&saddr,sizeof(saddr))>=0)
      break;
#ifdef DBUG
    else
      perror("bind failed");
#endif
    port++;
  }
  printf("Awaiting clients on port %d\n",port);

  const char* p = _tag;
  char* fromQname  = new char[128];
  XtcMonitorMsg::discoveryQueue(p, fromQname);
  struct mq_attr q_attr;
  q_attr.mq_maxmsg  = 2;
  q_attr.mq_msgsize = (long int)sizeof(XtcMonitorMsg);
  q_attr.mq_flags   = O_NONBLOCK;
  _discoveryQueue  = _openQueue(fromQname,q_attr);
  delete[] fromQname;
  _flushQueue(_discoveryQueue);

  if (::listen(fd,10)<0)
    printf("ConnectionManager listen failed\n");
  else {
    while(1) {
      timespec tv; tv.tv_sec=tv.tv_nsec=0;
      XtcMonitorMsg m(port);
      if (mq_timedsend(_discoveryQueue,(const char*)&m,sizeof(m),0,&_tmo)<0) {
	perror("Error advertising discovery port");
	abort();
      }

      sockaddr_in client;
      socklen_t   len = sizeof(sockaddr_in);
      int s = ::accept(fd,(sockaddr*)&client,&len);
      if (s<0) {
        perror("XtcMonitorServer discovery accept failed");
        abort();
      }

#ifdef DBUG
      printf("Accepted connection from %x.%d on socket %d\n",
	     ntohl(client.sin_addr.s_addr),ntohs(client.sin_port),s);
      printf("Writing to %d\n",_initFd);
#endif

      //  Post connection request to taskThread
      ::write(_initFd,&s,sizeof(s));
    }
  }
}

void XtcMonitorServer::_routine()
{
  while(1) {

    if (::poll(_pfd,_nfd,-1) > 0) {

      if (_pfd[0].revents & POLLIN) 
	_initialize_client();

      //
      //  Handle buffers returned from client
      //
      if (_pfd[1].revents & POLLIN) {
        _clearInput();
      }

      //
      //  Receive transitions back from clients
      //  or handle client disconnects
      //
      for(int i=2; i<_nfd; i++) {
	if (_pfd[i].revents & POLLIN) {
	  int r;
          XtcMonitorMsg msg;
	  while((r=::recv(_pfd[i].fd, (char*)&msg, sizeof(msg), MSG_DONTWAIT))>=0) {
	    for(unsigned q=0; q<_myTrFd.size(); q++)
	      if (_myTrFd[q]==_pfd[i].fd) {
		if (r > 0) {
                  Dgram* dg = reinterpret_cast<Dgram*>(_myShm+msg.bufferOffset());
		  if (_transitionCache->deallocate(dg,q))
		    _update(q,dg->seq.service());
		}
		else { // retire client
		  printf("Retiring client %d [%d]\n",q,_pfd[i].fd);
                  //  Recover buffers last sent to this client

                  _clearInput();
                  
                  //  Recover the buffers still queued to the retired client
                  _moveQueue(_myOutputEvQueue[q], _myInputEvQueue, q);

                  //  Force recovery of those still outstanding to the retired client
                  for(std::list<uint64_t>::iterator it=_msgDest[q].begin(); it!=_msgDest[q].end(); it++) {
                    printf("Recovering buffer %lx\n",*it);
                    msg.bufferOffset(*it);
                    if (mq_timedsend(_myInputEvQueue, (const char*)&msg, sizeof(msg), 0, &_tmo)<0)
                      perror("Failed to recover buffer queued to retired client");
                  }
                  _msgDest[q].clear();

		  _myTrFd[q]=-1;
                  //  Clear the transition tracking for this client
		  _transitionCache->deallocate(q);
		  _nfd--;
		  for(int j=i; j<_nfd; j++)
		    _pfd[j] = _pfd[j+1];
		  i--;
		}
		break;
	      }
	  }
	}
      }
    }
  }
}

static void* DiscRoutine(void* task)
{
  XtcMonitorServerF(task).discover();
  return 0;
}

static void* TaskRoutine(void* task)
{
  XtcMonitorServerF(task).routine();
  return 0;
}

char* XtcMonitorServer::_init() 
{ 
  const char* p = _tag;
  char* shmName    = new char[128];
  char* toQname    = new char[128];
  char* fromQname  = new char[128];

  sprintf(shmName  , "/PdsMonitorSharedMemory_%s",p);
  unsigned pageSize = (unsigned)sysconf(_SC_PAGESIZE);

  size_t sizeOfShm = size_t(_sizeofMem);
  unsigned remainder = sizeOfShm%pageSize;
  if (remainder) sizeOfShm += pageSize - remainder;

  umask(1);  // try to enable world members to open these devices.

  int shm = shm_open(shmName, OFLAGS, PERMS);
  if (shm < 0) { perror("shm_open"); return 0; }

  if ((ftruncate(shm, sizeOfShm))<0) { perror("ftruncate"); return 0; }

  _myShm = (char*)mmap(NULL, sizeOfShm, PROT_READ|PROT_WRITE, MAP_SHARED, shm, 0);
  if (_myShm == MAP_FAILED) { perror("mmap"); return 0; }

  _transitionCache = new TransitionCache(*this);

  mq_attr q_attr;

  q_attr.mq_maxmsg  = _numberOfEvBuffers;
  q_attr.mq_msgsize = (long int)sizeof(XtcMonitorMsg);
  q_attr.mq_flags   = O_NONBLOCK;

  XtcMonitorMsg::eventOutputQueue(p,_numberOfEvQueues-1,toQname);
  _flushQueue(_myInputEvQueue  = _openQueue(toQname,q_attr));

  q_attr.mq_maxmsg  = _numberOfEvBuffers / _numberOfEvQueues;
  q_attr.mq_msgsize = (long int)sizeof(XtcMonitorMsg);
  q_attr.mq_flags   = O_NONBLOCK;

  for(unsigned i=0; i<_numberOfEvQueues; i++) {
    XtcMonitorMsg::eventInputQueue(p,i,toQname);
    _flushQueue(_myOutputEvQueue[i] = _openQueue(toQname,q_attr));
  }

  { int pfd[2];
    if(::pipe(pfd)<0)
      perror("Opening pipe");
    
    _initFd         = pfd[1];
    _pfd[0].fd      = pfd[0];
    _pfd[0].events  = POLLIN;
    _pfd[0].revents = 0;
  }

  q_attr.mq_maxmsg  = _numberOfEvBuffers;
  q_attr.mq_msgsize = (long int)sizeof(XtcMonitorMsg);
  q_attr.mq_flags   = O_NONBLOCK;

  sprintf(toQname, "/PdsRequestQueue_%s",p);
  _requestQueue = _openQueue(toQname, q_attr);
  _flushQueue(_requestQueue);

  for(unsigned i=0; i<_numberOfEvBuffers; i++) {
    if (mq_timedsend(_requestQueue, (const char*)&_myMsg, sizeof(_myMsg), 0, &_tmo)<0)
      perror("Failed to queue buffer to input queue (initialize)");
  }

  _pfd[1].fd = _myInputEvQueue;
  _pfd[1].events  = POLLIN;
  _pfd[1].revents = 0;

  // create the listening threads
  pthread_create(&_taskThread,NULL,TaskRoutine,this);
  pthread_create(&_discThread,NULL,DiscRoutine,this);

  delete[] shmName;
  delete[] toQname;
  delete[] fromQname;

  return _myShm;
}

void XtcMonitorServer::_initialize_client()
{
#ifdef DBUG
  printf("Reading from %d\n",_pfd[0].fd);
#endif

  int s;
  if (::read(_pfd[0].fd,&s,sizeof(s))<0) {
    perror("Error reading client socket");
    abort();
  }

#ifdef DBUG
  printf("initialize client socket %d [%d]\n",s,_nfd);
#endif

  int iclient=-1;
  for(unsigned i=0; i<_myTrFd.size(); i++) {
    if (_myTrFd[i] == -1) {
      iclient = i;
      break;
    }
  }
  if (iclient == -1) {
    iclient = _myTrFd.size();
    if (iclient == int(_numberOfEvQueues)) {
      printf("Rejecting client %d : Number of EvQueues = %d\n",
             iclient, _numberOfEvQueues);
      ::close(s);
      return;
    }
    _myTrFd.push_back(-1);
  }

  _pfd[_nfd].fd = s;
  _pfd[_nfd].events  = POLLIN;
  _pfd[_nfd].revents = 0;
  _nfd++;

  _myTrFd[iclient] = s;
  printf("_initialize_client %d [socket %d]\n",iclient,s);
  
  _myMsg.sizeOfMem (_sizeofMem);
  _myMsg.queueIndex(iclient);
  
  if (::send(_myTrFd[iclient], (const char*)&_myMsg, sizeof(_myMsg), 0)<0) {
    perror("first send to client");
    abort();
  }

  _transitionCache->deallocate(iclient);
  _update(iclient,TransitionId::Unmap);
}

//
//  Send the cached transitions to update the client's state to the current DAQ state
//
void XtcMonitorServer::_update(int iclient,
			       TransitionId::Value last)
{
  XtcMonitorMsg myMsg(_myMsg);
  TransitionId::Value next = TransitionId::Value(last^1);
  std::list<Dgram*> tr(_transitionCache->current());
  for(std::list<Dgram*>::const_iterator it=tr.begin(); it!=tr.end(); it++) {
    if ((*it)->seq.service()>=next) {
      myMsg.bufferOffset(reinterpret_cast<char*>(*it)-_myShm);

      if (_transitionCache->allocate(*it,iclient))
        if (::send(_myTrFd[iclient], (const char*)&myMsg, sizeof(myMsg), 0)<0) {
          perror("Error sending current");
          _transitionCache->deallocate(*it,iclient);
        }
    }
  }
}

void XtcMonitorServer::_deleteDatagram(Dgram* p) {}

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
  else {  // Open twice to set all of the attributes
    printf("Opened queue %s (%d)\n",name,q);
  }

  mq_attr r_attr;
  mq_getattr(q,&r_attr);
  if (r_attr.mq_maxmsg != attr.mq_maxmsg ||
      r_attr.mq_msgsize!= attr.mq_msgsize) {

    printf("Failed to set queue attributes the first time.\n");
    mq_close(q);

    mqd_t q = mq_open(name,  O_CREAT|O_RDWR, PERMS, &attr);
    mq_getattr(q,&r_attr);

    if (r_attr.mq_maxmsg != attr.mq_maxmsg ||
	r_attr.mq_msgsize!= attr.mq_msgsize) {
      printf("Failed to set queue attributes the second time.\n");
      printf("open attr  %lx %lx %lx  read attr %lx %lx %lx\n",
	     attr.mq_flags, attr.mq_maxmsg, attr.mq_msgsize,
	     r_attr.mq_flags, r_attr.mq_maxmsg, r_attr.mq_msgsize);
    }
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
      mq_timedreceive(q, m, sz, NULL, &_tmo);
  } while (attr.mq_curmsgs);
}

void XtcMonitorServer::_moveQueue(mqd_t iq, mqd_t oq, unsigned q) 
{
  XtcMonitorMsg m;
  struct mq_attr attr;
  do {
    mq_getattr(iq, &attr);
    if (attr.mq_curmsgs) {
      if (mq_timedreceive(iq, (char*)&m, sizeof(m), NULL, &_tmo) == -1) {
        perror("moveQueue: mq_timedreceive");
        break;
      }
      else if (mq_timedsend   (oq, (char*)&m, sizeof(m), 0, &_tmo) == -1) {
        printf("Failed to reclaim buffer %lx : %s\n",
	       m.bufferOffset(), strerror(errno));
      }
      else
        _msgDest[q].remove(m.bufferOffset());
    }
  } while (attr.mq_curmsgs);
}

void XtcMonitorServer::_unlink() 
{ 
  printf("Unlinking Message Queues... \n");
  mq_close(_myInputEvQueue);

  for(unsigned i=0; i<_numberOfEvQueues; i++) {
    mq_close(_myOutputEvQueue[i]);
  }
  mq_close(_requestQueue);
  mq_close(_discoveryQueue);

  char* qname = new char[128];
  for(unsigned i=0; i<_numberOfEvQueues; i++) {
    XtcMonitorMsg::eventInputQueue     (_tag,i,qname); mq_unlink(qname);
  }
  XtcMonitorMsg::eventInputQueue     (_tag,_numberOfEvQueues,qname); mq_unlink(qname);
  sprintf(qname, "/PdsRequestQueue_%s",_tag);  mq_unlink(qname);
  XtcMonitorMsg::discoveryQueue      (_tag,qname); mq_unlink(qname);
  delete[] qname;
}

void XtcMonitorServer::_clearInput()
{
  XtcMonitorMsg msg;
  const timespec no_wait={0,0};
  while(mq_timedreceive(_myInputEvQueue, (char*)&msg, sizeof(msg), NULL, &no_wait) > 0) {
    _deleteDatagram(reinterpret_cast<Dgram*>(_myShm+msg.bufferOffset()));
    if (mq_timedsend(_requestQueue, (const char*)&msg, sizeof(msg), 0, &_tmo))
      perror("Writing to requestQ");
    for(unsigned i=0; i<_numberOfEvQueues; i++)
      _msgDest[i].remove(msg.bufferOffset());
  }
}
