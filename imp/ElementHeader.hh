//
//  Class for rectangular frame data
//
#ifndef Pds_Imp_ElementHeader_hh
#define Pds_Imp_ElementHeader_hh

#include <stdint.h>
//  0 0 ba 8c2b 1111 5555 3ff
namespace Pds {

  namespace Imp {

    class ElementHeader {
    public:
      ElementHeader();
      ~ElementHeader() {};
      enum frameTypes {FrameTypeDarkImage=0, FrameTypeImage=1};
    public:
      unsigned frameNumber()   const   { return ((unsigned)_frameNumber); }

    private:
      uint64_t _frameNumber;
      uint16_t _rangeCfg;
      uint16_t _calRangeCfg;
      uint16_t _numberOfSamples;
    };
  };
};
#endif
