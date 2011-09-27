//
//  Class for rectangular frame data
//
#ifndef Pds_FEXAMP_ElementV1_hh
#define Pds_FEXAMP_ElementV1_hh

#include "pdsdata/fexamp/ElementHeader.hh"

#include <stdint.h>

namespace Pds {

  namespace Fexamp {

    class ConfigV1;

    class ElementV1 : public ElementHeader {
    public:
      enum {Version=1};
      enum dimensions {RowsPerImage=1024, ColumnsPerElement=1};
      ElementV1() {};
      ~ElementV1() {};

    public:
      uint32_t       pixels[RowsPerImage][ColumnsPerElement];
      uint32_t       theLastWord;
    };
  };
};
#endif
