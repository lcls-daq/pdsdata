#include "pdsdata/cspad/ElementV1.hh"
#include "pdsdata/cspad/ConfigV1.hh"

using namespace Pds::CsPad;

ElementV1::ElementV1() 
{
}

unsigned ElementV1::virtual_channel    () const 
{
  return _word0&3; 
}

unsigned ElementV1::lane               () const 
{
  return (_word0>>6)&3; 
}

unsigned ElementV1::tid                () const 
{
  return _word0>>8; 
}

unsigned ElementV1::acq_count          () const 
{
  return _word1&0xffff; 
}

unsigned ElementV1::op_code            () const
{
  return (_word1>>16)&0xff;
}

unsigned ElementV1::quad               () const 
{
  return (_word1>>24)&3; 
}

unsigned ElementV1::seq_count          () const 
{
  return _seq_count; 
}

unsigned ElementV1::sb_temp            (unsigned i) const
{
  return _sbtemp[i]; 
}

unsigned ElementV1::frame_type         () const
{
  return _frame_type&0xff; 
}

unsigned ElementV1::ticks() const
{
  return _ticks;
}

unsigned ElementV1::fiducials() const
{
  return _fiducials;
}

const uint16_t* ElementV1::data() const
{
  return reinterpret_cast<const uint16_t*>(this+1);
}

const uint16_t* ElementV1::pixel(unsigned asic,
				 unsigned col,
				 unsigned row) const
{
  const uint16_t* d = data();     // quadrant
  d += ColumnsPerASIC*MaxRowsPerASIC*2*(asic>>1);  // advance to 2x1
  d += col*MaxRowsPerASIC*2;                // advance to column
  d += MaxRowsPerASIC*(asic&1);             // advance to ASIC
  d += row;                       // advance to row
  return d;
}

const ElementV1* ElementV1::next(const ConfigV1& c) const
{
  unsigned mask = c.asicMask();
  return reinterpret_cast<const ElementV1*>
    ( reinterpret_cast<const uint16_t*>(this+1)+ 
      (((mask&0xf) == 1) ? 4*ColumnsPerASIC*MaxRowsPerASIC : 
       ASICsPerQuad*ColumnsPerASIC*MaxRowsPerASIC) + 2 );
}
