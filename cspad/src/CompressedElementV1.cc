#include "pdsdata/cspad/CompressedElementV1.hh"
#include "pdsdata/cspad/ElementV1.hh"

using namespace Pds::CsPad;

CompressedElementV1::CompressedElementV1() 
{
}

CompressedElementV1::CompressedElementV1(const ElementV1& o) :
  ElementHeader(o)
{
}

const Pds::CompressedPayload& CompressedElementV1::pd() const
{ return _pd; }

