#ifndef Pds_TransitionCache_hh
#define Pds_TransitionCache_hh

//
//  TransitionCache class - the purpose of this class is to cache transitions for
//    clients and track the transitions which are still to be served to latent clients
//

#include "pdsdata/xtc/TransitionId.hh"

#include <list>
#include <map>

#include <semaphore.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

namespace Pds {
  class XtcMonitorServer;
  class Dgram;
  class TransitionCache {
  public:
    TransitionCache(XtcMonitorServer& srv);
    ~TransitionCache();
  public:
    void dump() const;
    std::list<Dgram*> current();
    void allocate  (Dgram* dg, TransitionId::Value);
    bool allocate  (Dgram* dg, unsigned client);
    bool deallocate(Dgram* dg, unsigned client);
    void deallocate(unsigned client);
    unsigned not_ready() const { return _not_ready; }
  private:
    XtcMonitorServer& _srv;
    sem_t            _sem;
    unsigned         _not_ready; // bitmask of clients that are behind in processing
    typedef std::map<Dgram*,uint32_t> AllocType;
    AllocType        _allocated; // bitmask of clients that were sent a transition
    typedef std::list<Dgram*> CacheType;
    CacheType        _cachedTr;  // set of transitions for the current DAQ state
  };
};

#endif
