//
//  Class for configuration of LBNL/ANL FCCD monochrome camera
//
#ifndef Fccd_ConfigV1_hh
#define Fccd_ConfigV1_hh

#include <stdint.h>

namespace Pds {
  namespace Camera {
    class FrameCoord;
  };

  namespace FCCD {

    class FccdConfigV1 {
    public:
      enum { Version=1 };
      enum Depth          { Sixteen_bit=16 };
      enum Output_Source  { Output_FIFO=0, Output_Pattern4=4 };

      // FCCD:
      //   Full Image size is 576 x 500 with 16 bit pixels
      enum { Row_Pixels=500 };
      enum { Column_Pixels=576 };

      FccdConfigV1();
      FccdConfigV1(
        uint16_t    u16OutputMode
      );

      uint16_t          outputMode()        const     { return _u16OutputMode; }
      unsigned          size()              const     { return (unsigned) sizeof(*this); }

private:
      uint16_t      _u16OutputMode;
    };

  };
};

#endif
