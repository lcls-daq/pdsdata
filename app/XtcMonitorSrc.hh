#ifndef Pds_XtcMonitorSrc_hh
#define Pds_XtcMonitorSrc_hh

#include "pdsdata/xtc/Src.hh"

#include <list>

namespace Pds {
  class XtcMonitorSrc {
  public:
    virtual ~XtcMonitorSrc() {}
  public:
    //  Set requested sources of data
    virtual void setSources(const std::list<Src>&)=0;
  };
};

#endif
