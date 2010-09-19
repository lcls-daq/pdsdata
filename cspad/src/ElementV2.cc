#include "pdsdata/cspad/ElementV2.hh"

using namespace Pds::CsPad;

ElementV2::ElementV2() 
{
}

unsigned ElementV2::virtual_channel    () const 
{
  return _word0&3; 
}

unsigned ElementV2::lane               () const 
{
  return (_word0>>6)&3; 
}

unsigned ElementV2::tid                () const 
{
  return _word0>>8; 
}

unsigned ElementV2::acq_count          () const 
{
  return _word1&0xffff; 
}

unsigned ElementV2::op_code            () const
{
  return (_word1>>16)&0xff;
}

unsigned ElementV2::quad               () const 
{
  return (_word1>>24)&3; 
}

unsigned ElementV2::seq_count          () const 
{
  return _seq_count; 
}

unsigned ElementV2::sb_temp            (unsigned i) const
{
  return _sbtemp[i]; 
}

unsigned ElementV2::frame_type         () const
{
  return _frame_type&0xff; 
}

unsigned ElementV2::ticks() const
{
  return _ticks;
}

unsigned ElementV2::fiducials() const
{
  return _fiducials;
}

