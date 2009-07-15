//
//  Class for Process Variable Control
//
#ifndef Pds_PVControl_hh
#define Pds_PVControl_hh

#include <stdint.h>

namespace Pds {

  namespace ControlData {

    class PVControl {
    public:
      enum { NameSize=32 };
    public:
      PVControl();
      PVControl(const char* pvname, double setValue);
      PVControl(const PVControl&);
      ~PVControl();
    public:
      const char* name () const;
      double      value() const;
    private:
      char     _name[NameSize];
      uint32_t _reserved;
      double   _value;
    };

  };

};

#endif
