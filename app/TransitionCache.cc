#include "pdsdata/app/TransitionCache.hh"
#include "pdsdata/app/XtcMonitorServer.hh"
#include "pdsdata/xtc/Dgram.hh"

#define DBUG

using namespace Pds;

TransitionCache::TransitionCache(XtcMonitorServer& srv) :
  _srv      (srv),
  _not_ready(0)
{ 
  sem_init(&_sem, 0, 1);
}    

TransitionCache::~TransitionCache() 
{
  sem_destroy(&_sem); 
}

void TransitionCache::dump() const {
  printf("---TransitionCache---\n");
  printf("\tBuffers:\n");
  for(AllocType::const_iterator it=_allocated.begin(); it!=_allocated.end(); it++) {
    const Dgram& odg = *(it->first);
    time_t t=odg.seq.clock().seconds();
    char cbuf[64]; ctime_r(&t,cbuf); strtok(cbuf,"\n");
    printf ("%15.15s : %s : %08x\n", 
            TransitionId::name(odg.seq.service()),
            cbuf,
            it->second);
  }
}

std::list<Dgram*> TransitionCache::current() {
  sem_wait(&_sem);
  std::list<Dgram*> ret(_cachedTr);
  sem_post(&_sem);
  return ret;
}

//
//  Find a free buffer for the next transition
//
void TransitionCache::allocate  (Dgram* dg, TransitionId::Value id) {
#ifdef DBUG
  printf("allocate(%s)\n",TransitionId::name(id));
#endif
  bool lbegin = ((id&1)==0);
  sem_wait(&_sem);

  {	  
    //
    //  Cache the transition for any clients 
    //    which may not be listening (yet)
    //
    if ( _cachedTr.empty() ) {
      if (id==TransitionId::Map) {
        _cachedTr.push_back(dg);
      }
      else {
        printf("Unexpected state for TransitionCache: _cachedTr empty but tr[%s]!=Map\n",
               TransitionId::name(id));
        //dump();
        //abort();
      }
    }
    else {
      const Dgram& odg = *_cachedTr.back();
      TransitionId::Value oid = odg.seq.service();
      if (id == oid+2) {       // Next begin transition
        _cachedTr.push_back(dg);
      }
      else if (id == oid+1) {  // Matching end transition
        if (_allocated.find(_cachedTr.back())==_allocated.end())
          _srv._deleteDatagram(_cachedTr.back());
        _cachedTr.pop_back();
      }
      else {  // unexpected transition
        printf("Unexpected transition for TransitionCache: tr[%s]!=[%s] or [%s]\n",
               TransitionId::name(id), 
               TransitionId::name(TransitionId::Value(oid+2)),
               TransitionId::name(TransitionId::Value(oid+1)));
        if (lbegin) { // Begin transition
          if (id > oid) {  // Missed a begin transition leading up to it
            printf("Irrecoverable.\n");
            dump();
            abort();
          }
          else {
            printf("Recover by rolling back.\n");
            do {
              Dgram* odg = _cachedTr.back();
              oid = odg->seq.service();
              if (_allocated.find(odg)==_allocated.end())
                _srv._deleteDatagram(odg);
              _cachedTr.pop_back();
            } while(oid > id);
            _cachedTr.push_back(dg);
          }
        }
        else { // End transition
          printf("Recover by rolling back.\n");
          Dgram* odg = _cachedTr.back();
          while( id < oid+3 ) {
            if (_allocated.find(odg)==_allocated.end())
              _srv._deleteDatagram(odg);
            _cachedTr.pop_back();
            if (_cachedTr.empty()) break;
            odg = _cachedTr.back();
            oid = odg->seq.service();
          }
        }
      }
    }

    if (lbegin) {
      unsigned not_ready=0;
      for(AllocType::const_iterator it=_allocated.begin(); it!=_allocated.end(); it++) {
        if (it->first == dg) continue;
        const Dgram& odg = *(it->first);
        if (odg.seq.service()==TransitionId::Enable)
          not_ready |= it->second;
      }

      if (not_ready &~_not_ready)
        printf("Transition %s: not_ready %x -> %x\n",
               TransitionId::name(id), _not_ready, _not_ready|not_ready);

      _not_ready |= not_ready;
    }

#ifdef DBUG
    printf("not_ready %08x\n",_not_ready);
#endif
  }

  sem_post(&_sem);
}

//
//  Queue this transition for a client
//
bool TransitionCache::allocate  (Dgram* dg, unsigned client) {

  bool result = true;
#ifdef DBUG
  printf("allocate[%p,%d] not_ready %08x\n",dg,client,_not_ready);
#endif

  sem_wait(&_sem);

  if (_not_ready & (1<<client)) {
    TransitionId::Value last=TransitionId::NumberOf;
    for(AllocType::const_iterator it=_allocated.begin(); it!=_allocated.end(); it++)
      if (it->second & (1<<client)) {
        TransitionId::Value td = it->first->seq.service();
        if ((td&1)==1 && td<last) last=td; // highest level "end/un" transition
      }
	
    //  Don't send transitions for lower cycles when not ready
    TransitionId::Value id = dg->seq.service();
    if (!((id&1)==1 && id<last))
      result=false;
  }

  if (result)
    _allocated[dg] |= (1<<client);

  sem_post(&_sem);

#ifdef DBUG
  printf("_allocated[%p] = %08x\n",dg,_allocated[dg]);
#endif 
  return result;
}

//
//  Client has completed this transition.
//  Remove client from _allocated list for this buffer.
//  Return true if client was previously "not ready" but now is "ready"
bool TransitionCache::deallocate(Dgram* dg, unsigned client) {
  bool result=false;
  sem_wait(&_sem);
  { unsigned v = _allocated[dg] & ~(1<<client);
#ifdef DBUG
    printf("_deallocate[%p,%d] %08x -> %08x\n",dg,client,
           _allocated[dg],v);
#else
    if ( _allocated[dg]==v )
      printf("_deallocate[%p,%d] %08x no change\n",dg,client,v);
#endif       
    if (v) 
      _allocated[dg]=v;
    else {
      _allocated.erase(dg);
      //  Delete if not cached 
      bool lfound=false;
      for(CacheType::const_iterator it=_cachedTr.begin(); it!=_cachedTr.end(); it++)
        if (*it == dg) { lfound=true; break; }
      if (!lfound)
        _srv._deleteDatagram(dg);
    }
  }

  if (_not_ready & (1<<client)) {
    for(AllocType::const_iterator it=_allocated.begin(); it!=_allocated.end(); it++)
      if (it->second & (1<<client)) {
        sem_post(&_sem);
        return false;
      }
    printf("not_ready %x -> %x\n", _not_ready, _not_ready&~(1<<client));
    _not_ready &= ~(1<<client);
    result=true;
  }
  sem_post(&_sem);
  return result;
}

//
//  Retire this client.
//  Remove the client from the _allocated list for all buffers.
//
void TransitionCache::deallocate(unsigned client) {
  sem_wait(&_sem);
  _not_ready &= ~(1<<client);
  for(AllocType::iterator it=_allocated.begin(); it!=_allocated.end(); it++) {
    it->second &= ~(1<<client);
    if (it->second==0) {
      Dgram* dg = it->first;
      _allocated.erase(it);
      //  Delete if not cached 
      bool lfound=false;
      for(CacheType::const_iterator cit=_cachedTr.begin(); cit!=_cachedTr.end(); cit++)
        if (*cit == dg) { lfound=true; break; }
      if (!lfound)
        _srv._deleteDatagram(dg);
    }
  }
  sem_post(&_sem);
#ifdef DBUG
  printf("deallocate %d\n",client);
  for(AllocType::const_iterator it=_allocated.begin(); it!=_allocated.end(); it++)
    printf("%08x ",it->second);
  printf("\n");
#endif
}
