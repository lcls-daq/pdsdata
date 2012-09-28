#ifndef Pds_TimePix_CompressedDataV1_hh
#define Pds_TimePix_CompressedDataV1_hh

#include "pdsdata/xtc/CompressedPayload.hh"

#include <stdint.h>
#include "pdsdata/xtc/TypeId.hh"

namespace Pds
{
   namespace Timepix
   {

     class DataV1;

     class CompressedDataV1
     {
     public:
       enum { Version = 1 };
       enum { Height = 512 };
       enum { Width = 512 };
       enum { Depth = 14 };
       enum { DepthBytes = 2 };
       enum { RawDataBytes = Height * Width * Depth / 8 };
       enum { DecodedDataBytes = Height * Width * DepthBytes };
       enum { MaxPixelValue = 11810 };

       CompressedDataV1() {}

       CompressedDataV1(const DataV1&);

       unsigned width() const
       { return (Width); }

       unsigned height() const
       { return (Height); }

       unsigned depth() const
       { return (Depth); }

       // number of bytes per pixel
       unsigned depth_bytes() const
       { return (DepthBytes); }

       // beginning of data appended to the end of this structure
       const unsigned char* data() const
       { return reinterpret_cast<const unsigned char*>(this+1); }

       uint32_t timestamp(void) const;
       uint16_t frameCounter(void) const;
       uint16_t lostRows(void) const;

       uint32_t _timestamp;
       uint16_t _frameCounter;
       uint16_t _lostRows;

     public:
       const CompressedPayload& pd() const;
     private:
       CompressedPayload _pd;
     };

     DataV1* uncompressed(const CompressedDataV1&);
   };
};

#endif
