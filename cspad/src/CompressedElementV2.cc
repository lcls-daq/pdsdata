#include "pdsdata/cspad/CompressedElementV2.hh"
#include "pdsdata/cspad/ElementV2.hh"

using namespace Pds::CsPad;

CompressedElementV2::CompressedElementV2() 
{
}

CompressedElementV2::CompressedElementV2(const ElementV2& o) :
  ElementHeader(o)
{
}

const Pds::CompressedPayload& CompressedElementV2::pd() const
{ return _pd; }

