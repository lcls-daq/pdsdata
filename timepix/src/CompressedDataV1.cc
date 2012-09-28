#include "pdsdata/timepix/CompressedDataV1.hh"
#include "pdsdata/timepix/DataV1.hh"

Pds::Timepix::CompressedDataV1::CompressedDataV1(const Pds::Timepix::DataV1& c) :
  _timestamp   (c._timestamp),
  _frameCounter(c._frameCounter),
  _lostRows    (c._lostRows)
{
}

uint32_t Pds::Timepix::CompressedDataV1::timestamp(void) const
{
  return (_timestamp);
}

uint16_t Pds::Timepix::CompressedDataV1::frameCounter(void) const
{
  return (_frameCounter);
}

uint16_t Pds::Timepix::CompressedDataV1::lostRows(void) const
{
  return (_lostRows);
}

const Pds::CompressedPayload& Pds::Timepix::CompressedDataV1::pd() const
{
  return _pd;
}
