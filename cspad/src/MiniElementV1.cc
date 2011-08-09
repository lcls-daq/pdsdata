#include "pdsdata/cspad/MiniElementV1.hh"

using namespace Pds::CsPad;

MiniElementV1::MiniElementV1() 
{
}

uint16_t MiniElementV1::pixel(unsigned asic,
                              unsigned col,
                              unsigned row) const
{
  unsigned r = ((asic&1)*MaxRowsPerASIC+row)>>1;
  if ((asic&2)==0) {
    if ((row&1)==0)
      return pair[col][r].s0_a;
    else
      return pair[col][r].s0_b;
  }
  else if ((row&1)==0)
    return pair[col][r].s1_a;
  else
    return pair[col][r].s1_b;
}
