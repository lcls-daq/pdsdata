#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#ifdef _POSIX_MESSAGE_PASSING
#include <mqueue.h>
#endif

#include <sys/socket.h>
#include <arpa/inet.h>

#include "pdsdata/xtc/DetInfo.hh"
#include "pdsdata/xtc/ProcInfo.hh"
#include "pdsdata/xtc/XtcIterator.hh"
#include "pdsdata/xtc/Dgram.hh"
#include "XtcMonitorClient.hh"
#include "XtcMonitorMsg.hh"

#include <poll.h>

//#define DBUG

enum {PERMS = S_IRUSR|S_IRGRP|S_IROTH|S_IWUSR|S_IWGRP|S_IWOTH};
enum {PERMS_IN  = S_IRUSR|S_IRGRP|S_IROTH};
enum {PERMS_OUT  = S_IWUSR|S_IWGRP|S_IWOTH};
enum {OFLAGS = O_RDONLY};

static mqd_t _openQueue(const char* name, unsigned flags, unsigned perms,
                        bool lwait=true)
{
  struct mq_attr mymq_attr;
  mqd_t queue;
  while(1) {
    queue = mq_open(name, flags, perms, &mymq_attr);
    if (queue == (mqd_t)-1) {
      char b[128];
      sprintf(b,"mq_open %s",name);
      perror(b);
      sleep(1);
      if (!lwait) break;
    }
    else {
      printf("Opened queue %s (%d)\n",name,queue);
      break;
    }
  }
  return queue;
}

namespace Pds {
  class DgramHandler {
  public:
    DgramHandler(XtcMonitorClient& client, 
		 int trfd, mqd_t evqin, mqd_t evqout, unsigned ev_index,
	      const char* tag, char* myShm) :
      _client(client), 
      _trfd(trfd), _evqin(evqin), _evqout(evqout), _ev_index(ev_index),
      _tag(tag), _shm(myShm), _last(TransitionId::Reset)
    {
      _tmo.tv_sec = _tmo.tv_nsec = 0;
    }
    ~DgramHandler() {}
  public:
    enum Request { Continue, Reconnect, Return };
    Request transition() { 
      XtcMonitorMsg myMsg;
      int nb = ::recv(_trfd, (char*)&myMsg, sizeof(myMsg), 0);
      if (nb < 0) {
	perror("transition receive");
	return Continue;
      }
      if (nb == 0) {
#ifdef DBUG
	printf("Received tr disconnect [%d]\n",_trfd);
#endif
	return Reconnect;
      }
      unsigned offs = myMsg.bufferOffset();
#ifdef DBUG
      printf("Received tr buffer offs 0x%x [%p]\n",offs,_shm+offs);
#endif
      {
	Dgram* dg = (Dgram*) (_shm + offs);
	_last = dg->seq.service();
	if (_client.processDgram(dg))
	  return Return;
#ifdef DBUG
	printf("Returning tr buffer offs 0x%x [%p]\n",offs,dg);
#endif
	if (::send(_trfd,(char*)&myMsg,sizeof(myMsg),0)<0) {
	  perror("transition send");
	  return Return;
	}
      }
      return Continue;
    }
    Request event     () { 
      mqd_t  iq = _evqin;
      mqd_t  oq = _evqout;

      XtcMonitorMsg myMsg;
      unsigned priority(0);
      int nb;
      if ((nb=mq_receive(iq, (char*)&myMsg, sizeof(myMsg), &priority)) < 0) {
	perror("mq_receive buffer");
	return Return;
      } 
      else {
        unsigned offs = myMsg.bufferOffset();
#ifdef DBUG
	printf("Received ev buffer 0x%x [%p]\n",offs,_shm+offs);
#endif
        {
	  Dgram* dg = (Dgram*) (_shm + offs);
	  if (_last==TransitionId::Enable &&
	      _client.processDgram(dg))
	    return Return;
          if (mq_timedsend(oq, (const char *)&myMsg, sizeof(myMsg), priority, &_tmo)) {
            ;
	  }
	}
      }
      return Continue;
    }
  private:
    XtcMonitorClient& _client;
    int               _trfd;
    mqd_t             _evqin;
    mqd_t             _evqout;
    unsigned          _ev_index;
    const char*       _tag;
    char*             _shm;
    timespec          _tmo;
    TransitionId::Value _last;
  };
};

using namespace Pds;

int XtcMonitorClient::processDgram(Dgram* dg) {
  printf("%s transition: time 0x%lx, payloadSize 0x%x\n",TransitionId::name(dg->seq.service()),
      dg->seq.stamp().fiducials(),dg->xtc.sizeofPayload());
  return 0;
}

std::list<Src> XtcMonitorClient::select(const std::list<Src>& src)
{ return src; }

int XtcMonitorClient::run(const char* tag, int tr_index) 
{ return run(tag, tr_index, tr_index); }

int XtcMonitorClient::run(const char* tag, int tr_index, int) {
  int error = 0;
  char* qname             = new char[128];

  umask(0);   // Need this to set group/other write permissions on mqueue

  XtcMonitorMsg myMsg;
  unsigned priority;

  mqd_t myOutputEvQueue = (mqd_t)-1;

  //
  //  Request initialization
  //

  while(1) {
    int myTrFd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (myTrFd < 0) {
      perror("Opening myTrFd socket");
      return 1;
    }

    while(1) {
      XtcMonitorMsg::discoveryQueue(tag,qname);
      mqd_t discoveryQueue = _openQueue(qname, O_RDONLY, PERMS_IN);
      if (discoveryQueue == (mqd_t)-1)
	error++;
      
      if (mq_receive(discoveryQueue, (char*)&myMsg, sizeof(myMsg), &priority) < 0) {
	perror("mq_receive discoveryQ");
	return ++error;
      } 
      
      mq_close(discoveryQueue);

      sockaddr_in saddr;
      saddr.sin_family = AF_INET;
      saddr.sin_addr.s_addr = htonl(0x7f000001);
      saddr.sin_port        = htons(myMsg.trPort());
      
      if (::connect(myTrFd, (sockaddr*)&saddr, sizeof(saddr)) < 0) {
	perror("Connecting myTrFd socket");
	sleep(1);
      }
      else {
#ifdef DBUG
        socklen_t addrlen = sizeof(sockaddr_in);
        sockaddr_in name;
        ::getsockname(myTrFd, (sockaddr*)&name, &addrlen);
	printf("Connected to %08x.%d [%d] from %08x.%d\n", 
               ntohl(saddr.sin_addr.s_addr),ntohs(saddr.sin_port),myTrFd,
               ntohl(name.sin_addr.s_addr),ntohs(name.sin_port));
#endif
	break;
      }
    }

    if (::read(myTrFd,&myMsg,sizeof(myMsg))!=sizeof(myMsg)) {
      printf("Connection rejected by shmem server [too many clients]\n");
      return 1;
    }

    //
    //  Initialize shared memory from first message
    //
    unsigned sizeOfShm = myMsg.sizeOfMem();
    unsigned pageSize  = (unsigned)sysconf(_SC_PAGESIZE);
    unsigned remainder = sizeOfShm % pageSize;
    if (remainder)
      sizeOfShm += pageSize - remainder;

    XtcMonitorMsg::sharedMemoryName(tag, qname);
    printf("Opening shared memory %s of size 0x%x\n",
	   qname,sizeOfShm);

    int shm = shm_open(qname, OFLAGS, PERMS_IN);
    if (shm < 0) perror("shm_open");
    char* myShm = (char*)mmap(NULL, sizeOfShm, PROT_READ, MAP_SHARED, shm, 0);
    if (myShm == MAP_FAILED) perror("mmap");
    else printf("Shared memory at %p\n", (void*)myShm);
  
    int ev_index = myMsg.queueIndex();
    XtcMonitorMsg::eventInputQueue(tag,ev_index,qname);
    mqd_t myInputEvQueue = _openQueue(qname, O_RDONLY, PERMS_IN);
    if (myInputEvQueue == (mqd_t)-1)
      error++;

    XtcMonitorMsg::eventInputQueue(tag,myMsg.return_queue(),qname);
    myOutputEvQueue = _openQueue(qname, O_WRONLY, PERMS_OUT);
    if (myOutputEvQueue == (mqd_t)-1)
      error++;
  
    if (error) {
      fprintf(stderr, "Could not open at least one message queue!\n");
      fprintf(stderr, "tag %s, tr_index %d, ev_index %d\n",tag,tr_index,ev_index);
      return error;
    }


    //
    //  Seek the Map transition
    //
    do {
      if (::recv(myTrFd, (char*)&myMsg, sizeof(myMsg), MSG_WAITALL) < 0) {
	perror("mq_receive buffer");
	return ++error;
      } 
      else {
        unsigned offs = myMsg.bufferOffset();
        {
	  Dgram* dg = (Dgram*) (myShm + offs);
	  if (dg->seq.service()==TransitionId::Map) {
	    if (!processDgram(dg)) {
	      if (::send(myTrFd,(char*)&myMsg,sizeof(myMsg),0)<0) {
		perror("transition send");
		return false;
	      }
	      break;
	    }
          } 
          else
            printf("Unexpected transition %s != Map\n",TransitionId::name(dg->seq.service()));
	}
      }
    } while(1);

    //
    //  Handle all transitions first, then events
    //
    pollfd pfd[2];
    pfd[0].fd      = myTrFd;
    pfd[0].events  = POLLIN | POLLERR;
    pfd[0].revents = 0;
    pfd[1].fd      = myInputEvQueue;
    pfd[1].events  = POLLIN | POLLERR;
    pfd[1].revents = 0;
    int nfd = 2;

    DgramHandler handler(*this,
			 myTrFd,
			 myInputEvQueue,myOutputEvQueue,ev_index,
			 tag,myShm);

    DgramHandler::Request r=DgramHandler::Continue;
    while (r==DgramHandler::Continue) {
      if (::poll(pfd, nfd, -1) > 0) {
	if (pfd[0].revents & POLLIN) { // Transition
	  r = handler.transition();
	}
	else if (pfd[1].revents & POLLIN) { // Event
	  r = handler.event     ();
	}
      }
    }

    close(myTrFd);

    if (r==DgramHandler::Return) return 1;
  }
}

