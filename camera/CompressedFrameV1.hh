#ifndef Pds_CompressedFrameV1_hh
#define Pds_CompressedFrameV1_hh

#include "pdsdata/xtc/CompressedPayload.hh"

#include <stdint.h>

namespace Pds {
  namespace Camera {
    class FrameV1;
    class CompressedFrameV1 {
    public:
      CompressedFrameV1() {}
      CompressedFrameV1(const FrameV1&);
    public:
      unsigned                width () const;
      unsigned                height() const;
      unsigned                depth () const;
      unsigned                offset() const;
    public:
      const CompressedPayload& pd() const;
    private:
      uint32_t         _width;
      uint32_t         _height;
      uint32_t         _depth;
      uint32_t         _offset;
    private:
      CompressedPayload _pd;
    };

    FrameV1* uncompressed(const CompressedFrameV1&);
  };
};

#endif
