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
    ///  Called prior to any of the configure() methods
    virtual void pre_configure (const std::string& input_data) {}
    ///  Called after all of the configure() methods
    virtual bool post_configure() { return true; }
    /**
     **  The data (payload) passed in the configure method is only
     **  guaranteed to be valid during the call.  The derived class
     **  must copy the data if it will be needed later.
     **/
    virtual void configure(const Pds::DetInfo&   src,
			   const Pds::TypeId&    type,
			   void*                 payload) = 0;
    virtual void configure(const Pds::BldInfo&   src,
			   const Pds::TypeId&    type,
			   void*                 payload) = 0;
    virtual void configure(const Pds::ProcInfo&  src,
			   const Pds::TypeId&    type,
			   void*                 payload) = 0;

    ///  Called prior to any of the event() methods
    virtual void pre_event () {}

    /**
     **  The data (payload) passed in the event method is
     **  guaranteed to be valid until the accept method is called.
     **  No reference to this data shall be made after that time.
     **/
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
    ///  Return a description of the module including any software tag information
    virtual std::string name() const = 0; 
    ///  Return a description of the modules configuration.
    ///  Should be sufficient to reproduce the modules configuration
   virtual std::string configuration() const = 0;
    ///  Return true if all required information has been received in the event() methods
    ///  independent of the accept() result
    virtual bool complete() { return false; }
    ///  Return the filter decision for the event data
    virtual bool accept  () { return true; }
  };
};

typedef Pds::L3FilterModule* create_m();

#endif
