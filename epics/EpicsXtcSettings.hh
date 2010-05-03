#ifndef EPICS_XTC_SETTINGS_H
#define EPICS_XTC_SETTINGS_H
#include "pdsdata/xtc/TypeId.hh"
#include "pdsdata/xtc/DetInfo.hh"

namespace Pds
{
    
namespace EpicsXtcSettings
{
  const int            iXtcVersion = 1;    
  const int            iMaxXtcSize = 2000000; // Space enough for 10000 PVs of type DBR_DOUBLE
  const TypeId::Type   typeXtc     = TypeId::Id_Epics;
  extern const DetInfo detInfo;
}

}

#endif
