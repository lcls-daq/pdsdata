//
//  Class for rectangular frame data
//
#ifndef Pds_ElementV1_hh
#define Pds_ElementV1_hh

#include <stdint.h>

namespace Pds {

  namespace Cspad {

    class ElementV1 {
    public:
      enum {Version=1};
      enum { ASICS=16, Columns=185, Rows=194 };
      ElementV1();
    public:
      unsigned virtual_channel    () const;
      unsigned lane               () const;
      unsigned tid                () const;
      unsigned acq_count          () const;
      unsigned op_code            () const;
      unsigned quad               () const;
      unsigned seq_count          () const;
      unsigned long long beam_code() const;
      unsigned sb_temp            (unsigned i) const;
      unsigned frame_type         () const;

      //  beginning of pixel data
      const uint16_t* data() const;
      //  location of individual pixel datum
      const uint16_t* pixel(unsigned asic, unsigned column, unsigned row) const;
    private:
      uint32_t _word0;
      uint32_t _word1;
      uint32_t _seq_count;
      uint32_t _beam_code[2];
      uint16_t _sbtemp[4];
      uint32_t _frame_type;
      uint16_t _data[ASICS*Columns*Rows];
    };
  };
};
#endif