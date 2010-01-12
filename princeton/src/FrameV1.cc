#include "pdsdata/princeton/FrameV1.hh"
#include "pdsdata/princeton/ConfigV1.hh"

using namespace Pds;
using namespace Princeton;

const uint16_t* FrameV1::data() const
{
  return (uint16_t*) (this + 1);
}
