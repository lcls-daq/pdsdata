#ifndef L3FilterModule_hh
#define L3FilterModule_hh

/**
 **  Class L3FilterModule - a base class for user implementation of an
 **  event filter.  The subclass of this module shall be compiled into
 **  a shared library which will be dynamicly loaded by the recording
 **  process on a DAQ DSS node.  The shared library must have a "create()"
 **  method which returns a new instanciation of the subclass.
 **
 **  The module receives access to the data via callbacks of the
 **  "configure()" and "event()" methods.  The filter result is queried
 **  by the "accept()" method callback.
 **/

#include "pdsdata/xtc/DetInfo.hh"
#include "pdsdata/xtc/BldInfo.hh"
#include "pdsdata/xtc/ProcInfo.hh"
#include "pdsdata/xtc/TypeId.hh"

#include <string>

namespace Pds {
  class L3FilterModule {
  public:
    virtual ~L3FilterModule() {}
  public:
    virtual void configure(const Pds::DetInfo&   src,
			   const Pds::TypeId&    type,
			   void*                 payload) = 0;
    virtual void configure(const Pds::BldInfo&   src,
			   const Pds::TypeId&    type,
			   void*                 payload) = 0;
    virtual void configure(const Pds::ProcInfo&  src,
			   const Pds::TypeId&    type,
			   void*                 payload) = 0;
    virtual void event    (const Pds::DetInfo&   src,
			   const Pds::TypeId&    type,
			   void*                 payload) = 0;
    virtual void event    (const Pds::BldInfo&   src,
			   const Pds::TypeId&    type,
			   void*                 payload) = 0;
    virtual void event    (const Pds::ProcInfo&  src,
			   const Pds::TypeId&    type,
			   void*                 payload) = 0;
  public:
    virtual std::string name() const = 0;
    virtual std::string configuration() const = 0;
    virtual bool accept () { return true; }
  };
};

typedef Pds::L3FilterModule* create_m();

#endif
