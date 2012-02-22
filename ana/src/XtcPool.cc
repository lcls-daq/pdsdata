#include "pdsdata/ana/XtcPool.hh"
#include "pdsdata/xtc/Dgram.hh"

namespace Pds {
  namespace Ana {

    XtcPool::XtcPool(unsigned nevents, unsigned eventsize) :
      _eventsize(eventsize) {
      while(nevents--) {
        char* b = new char[eventsize];
        _free.push(b);
      }
    }

    XtcPool::~XtcPool() {
    }

    //
    //  Read a complete event into the pool from file descriptor 'fd'.
    //  Upon failure, insert 0 into the pool and return false
    //  For 'live' file read, always return true.
    //
    bool XtcPool::push(int fd) {
      char* b = _free.pop();
      if (b == NULL) {
        return false;
      }
  
      unsigned sz = sizeof(Pds::Dgram);
      ssize_t rsz = ::read(fd, b, sz);
      if (rsz != ssize_t(sz) && _live && rsz > 0) {
        _waitAndFill(fd, b+rsz, sz-rsz);
        rsz = ssize_t(sz);
      }

      if (rsz == ssize_t(sz)) {
        Pds::Dgram* dg = reinterpret_cast<Pds::Dgram*>(b);
        sz = dg->xtc.sizeofPayload();
    
#ifdef DUMP_DMGOFF
        if (dg->xtc.damage.value()&(1<<Pds::Damage::DroppedContribution)) {
          off64_t pos = ::lseek64(fd,0,SEEK_CUR);
          char buff[128];
          time_t t = dg->seq.clock().seconds();
          strftime(buff,128,"%H:%M:%S",localtime(&t));
          printf("%s.%09u dmg %08x @ %llu\n",
                 buff,dg->seq.clock().nanoseconds(),dg->xtc.damage.value(),pos);
        }
#endif
        if (sz + sizeof(Pds::Dgram) > _eventsize) {
          printf("Event size (%d) is greater than pool size(%d); skipping remainder of file\n", sz, _eventsize);
        } else {
          rsz = ::read(fd, dg->xtc.payload(), sz);
          if (rsz == ssize_t(sz)) {
            _pend.push(b);
            return true;
          } else if (_live) {
            if (rsz==-1) {
              perror("Error reading file");
              exit(1);
            }
            _waitAndFill(fd, dg->xtc.payload()+rsz, sz-rsz);
          }
        }
      }

      _free.push(b);
      _pend.push(0);

      if (_live) {
        printf("\rLive read waits...");
        fflush(stdout);
        sleep(1);
        return true;
      }

      return false;
    }

    Pds::Dgram* XtcPool::pop(Pds::Dgram* r) {
      if (r != NULL) {
        _free.push(reinterpret_cast<char*>(r));
      }
      return reinterpret_cast<Pds::Dgram*>(_pend.pop());
    }

    void XtcPool::unblock() {
      _free.unblock();
      _pend.unblock();
    }  

    void XtcPool::_waitAndFill(int fd, char* p, unsigned sz) {
      do {
        printf("\rLive read waits... (%07d)",sz);
        fflush(stdout);
        sleep(1);
        ssize_t rsz = ::read(fd, p, sz);
        p  += rsz;
        sz -= rsz;
      } while (sz);
    }
  }
}
