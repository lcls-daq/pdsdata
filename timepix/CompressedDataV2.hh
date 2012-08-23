#ifndef Pds_CompressedDataV2_hh
#define Pds_CompressedDataV2_hh

#include <stdint.h>

namespace Pds {
  namespace TimePix {
    class DataV2;
    class CompressedDataV2 {
    public:
      enum {Version=1};
      CompressedDataV2() {}
      CompressedDataV2(const DataV2&);      
    public:
      unsigned                width       () const;
      unsigned                height      () const;
      unsigned                timestamp   () const;
      unsigned                frameCounter() const;
      unsigned                lostRows    () const;
      const void*             data  () const;
    public:
      void*                   data  ();
    private:
      uint16_t _width;        // pixels per row
      uint16_t _height;       // pixels per column
      uint32_t _timestamp;    // hardware timestamp
      uint16_t _frameCounter; // hardware frame counter
      uint16_t _lostRows;     // lost row count
    };
  };
};

#endif
