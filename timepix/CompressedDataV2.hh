#ifndef Pds_TimePix_CompressedDataV2_hh
#define Pds_TimePix_CompressedDataV2_hh

#include "pdsdata/xtc/CompressedPayload.hh"

#include <stdint.h>

namespace Pds {
  namespace Timepix {
    class DataV2;
    class CompressedDataV2 {
    public:
      CompressedDataV2() {}
      CompressedDataV2(const DataV2&);      
    public:
      unsigned                width       () const;
      unsigned                height      () const;
      unsigned                timestamp   () const;
      unsigned                frameCounter() const;
      unsigned                lostRows    () const;
    public:
      const CompressedPayload& pd() const;
    private:
      uint16_t _width;        // pixels per row
      uint16_t _height;       // pixels per column
      uint32_t _timestamp;    // hardware timestamp
      uint16_t _frameCounter; // hardware frame counter
      uint16_t _lostRows;     // lost row count
    private:
      CompressedPayload _pd;
    };

    DataV2* uncompressed(const CompressedDataV2&);
  };
};

#endif
