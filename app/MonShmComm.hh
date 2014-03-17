#ifndef Pds_MonShmComm_hh
#define Pds_MonShmComm_hh

#include <stdint.h>
#include <string.h>

namespace Pds {
  namespace MonShmComm {

    enum { ServerPort = 5719 };

    class Get {
    public:
      char     hostname[32];
      uint32_t groups;
      uint32_t mask;
      uint32_t events;
      uint32_t dmg;
    };

    class Set {
    public:
      Set() {}
      Set(const char* n,unsigned m) : mask(m) { strncpy(hostname,n,32); }
    public:
      char     hostname[32];
      uint32_t mask;
    };
  };
};

#endif
