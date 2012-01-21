// $Id: DataV1.hh,v 1.6 2012-01-21 03:27:05 caf Exp $
// Author: Chris Ford <caf@slac.stanford.edu>

//
//  Class for data of Timepix
//
//  The Timepix Quad detector includes 4 256x256 elements.
//  Software should treat it as 512x512 elements of 2 bytes each.
//
//  The appended data size is exported by the data_size() method.
//
#ifndef TIMEPIXDATAV1_HH
#define TIMEPIXDATAV1_HH

#include <stdint.h>
#include "pdsdata/xtc/TypeId.hh"

#define TIMEPIX_HEIGHT          1024
#define TIMEPIX_WIDTH           256
#define TIMEPIX_DEPTH           14
#define TIMEPIX_DEPTH_BYTES     2
#define TIMEPIX_RAW_DATA_BYTES \
          ((TIMEPIX_HEIGHT) * (TIMEPIX_WIDTH) * (TIMEPIX_DEPTH) / 8)

#define TIMEPIX_DECODED_DATA_BYTES \
          ((TIMEPIX_HEIGHT) * (TIMEPIX_WIDTH) * (TIMEPIX_DEPTH_BYTES))

namespace Pds
{
   namespace Timepix
   {
      class DataV1;
   }
}

class Pds::Timepix::DataV1
{
  public:
    enum { Version = 1 };

    DataV1() :
      _timestamp(0),
      _frameCounter(0),
      _lostRows(0)
      {}

    DataV1(uint32_t timestamp, uint16_t frameCounter, uint16_t lostRows) :
      _timestamp(timestamp),
      _frameCounter(frameCounter),
      _lostRows(lostRows)
      {}

    static Pds::TypeId typeId()
      { return TypeId(TypeId::Id_TimepixData, Version); }

    unsigned width() const
      { return (TIMEPIX_WIDTH); }

    unsigned height() const
      { return (TIMEPIX_HEIGHT); }

    unsigned depth() const
      { return (TIMEPIX_DEPTH); }

    // number of bytes per pixel
    unsigned depth_bytes() const
      { return (TIMEPIX_DEPTH_BYTES); }

    // size of data appended to the end of this structure
    unsigned data_size() const
      { return (TIMEPIX_DECODED_DATA_BYTES); }

    // beginning of data appended to the end of this structure
    const unsigned char* data() const
      { return reinterpret_cast<const unsigned char*>(this+1); }

    uint32_t timestamp(void) const;
    uint16_t frameCounter(void) const;
    uint16_t lostRows(void) const;

    uint32_t _timestamp;
    uint16_t _frameCounter;
    uint16_t _lostRows;
};

#endif
