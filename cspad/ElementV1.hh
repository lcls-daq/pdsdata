//
//  Class for rectangular frame data
//
#ifndef Pds_ElementV1_hh
#define Pds_ElementV1_hh

#include <stdint.h>

namespace Pds {

  namespace CsPad {

    class ConfigV1;
    class ConfigV2;

    class ElementV1 {
    public:
      enum {Version=1};
      ElementV1();
    public:
      unsigned virtual_channel    () const;
      unsigned lane               () const;
      unsigned tid                () const;
      unsigned acq_count          () const;
      unsigned op_code            () const;
      unsigned quad               () const;
      unsigned seq_count          () const;
      unsigned ticks              () const;
      unsigned fiducials          () const;
      unsigned sb_temp            (unsigned i) const;
      unsigned frame_type         () const;

      //  beginning of pixel data
      const uint16_t* data() const;

      //  location of individual pixel datum
      const uint16_t* pixel(unsigned asic, unsigned column, unsigned row) const;
      const ElementV1* next(const ConfigV1&) const;

      const uint16_t* pixel(unsigned asic, unsigned column, unsigned row,
			    const ConfigV2&) const;
      const ElementV1* next(const ConfigV2&) const;

    private:
      uint32_t _word0;
      uint32_t _word1;
      uint32_t _seq_count;
      uint32_t _ticks;
      uint32_t _fiducials;
      uint16_t _sbtemp[4];
      uint32_t _frame_type;
    };
  };
};
#endif
