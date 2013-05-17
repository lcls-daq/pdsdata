//
//  Class for rectangular frame data
//
#ifndef Pds_IMP_ElementV1_hh
#define Pds_IMP_ElementV1_hh

#include "pdsdata/imp/ElementHeader.hh"
#include "pdsdata/imp/ConfigV1.hh"

#include <stdint.h>

#pragma pack(4)

namespace Pds {

  namespace Imp {

    class ConfigV1;
    typedef Pds::Imp::ConfigV1  ImpConfigType;

    enum {channelsPerDevice=4};  // assumes power of 2

    class Sample {
      public:
        Sample() {};
        ~Sample() {};

        uint16_t channel(unsigned i) { return (_value[ i & (channelsPerDevice-1)]); }
        const uint16_t channel(unsigned i) const { return (_value[ i & (channelsPerDevice-1)]); }
      private:
        uint16_t _value[channelsPerDevice];
    };

    class ElementV1 : public ElementHeader {
      public:
        enum {Version=1, Uint32sPerHeader=8, Uint32sPerSample=2};

        ElementV1() {};
        ~ElementV1() {};

        Sample getSample(unsigned index) {
          uint16_t* u = (uint16_t*)this;
          Sample* dat = (Sample*) (u + (sizeof(ElementV1)/sizeof(uint16_t)));
          return dat[index];
        }
        const Sample getSample(unsigned index) const {
          uint16_t* u = (uint16_t*)this;
          Sample* dat = (Sample*) (u + (sizeof(ElementV1)/sizeof(uint16_t)));
          return dat[index];
        }
    };
  };
};

#endif
