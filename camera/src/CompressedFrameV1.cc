#include "pdsdata/camera/CompressedFrameV1.hh"
#include "pdsdata/camera/FrameV1.hh"

using namespace Pds::Camera;

CompressedFrameV1::CompressedFrameV1(const FrameV1& f) :
  _width (f.width ()),
  _height(f.height()),
  _depth (f.depth ()),
  _offset(f.offset())
{
}

unsigned                 CompressedFrameV1::width () const { return _width ; }
unsigned                 CompressedFrameV1::height() const { return _height; }
unsigned                 CompressedFrameV1::depth () const { return _depth ; }
unsigned                 CompressedFrameV1::offset() const { return _offset; }

const Pds::CompressedPayload& CompressedFrameV1::pd    () const { return _pd; }
