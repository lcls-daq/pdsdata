#include "pdsdata/princeton/FrameV2.hh"

using namespace Pds;
using namespace Princeton;

FrameV2::FrameV2( uint32_t iShotIdStart, float fReadoutTime ) :
 _iShotIdStart(iShotIdStart), _fReadoutTime(fReadoutTime), _fTemperature(TemperatureNotDefined)
{
}

const uint16_t* FrameV2::data() const
{
  return (uint16_t*) (this + 1);
}
