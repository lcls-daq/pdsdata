//
//  Class for Process Variable Monitoring
//
#ifndef Pds_PVMonitor_hh
#define Pds_PVMonitor_hh

#include <stdint.h>

namespace Pds {

  namespace ControlData {

    class PVMonitor {
    public:
      enum { NameSize=32 };
      enum AlarmType { Low =0x1, 
		       High=0x2, 
		       LoLo=0x4, 
		       HiHi=0x8 };  // maps to PV alarms
    public:
      PVMonitor();
      PVMonitor(const char* pvname, unsigned restrictedAlarms);
      PVMonitor(const PVMonitor&);
      ~PVMonitor();
    public:
      const char* name            () const;
      unsigned    restrictedAlarms() const;
    private:
      char     _name[NameSize];
      uint32_t _restricted;
    };

  };

};

#endif
