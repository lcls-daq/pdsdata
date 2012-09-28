#include "pdsdata/timepix/CompressedDataV2.hh"
#include "pdsdata/timepix/DataV2.hh"

using namespace Pds::Timepix;

CompressedDataV2::CompressedDataV2(const DataV2& d) :
  _width       (d.width       ()),
  _height      (d.height      ()),
  _timestamp   (d.timestamp   ()),
  _frameCounter(d.frameCounter()),
  _lostRows    (d.lostRows    ())
{
}

unsigned                CompressedDataV2::width       () const { return _width; }
unsigned                CompressedDataV2::height      () const { return _height; }
unsigned                CompressedDataV2::timestamp   () const { return _timestamp; }
unsigned                CompressedDataV2::frameCounter() const { return _frameCounter; }
unsigned                CompressedDataV2::lostRows    () const { return _lostRows; }

const Pds::CompressedPayload& CompressedDataV2::pd() const { return _pd; }
