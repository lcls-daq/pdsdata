#ifndef Pds_FrameV1_hh
#define Pds_FrameV1_hh

#include <stdint.h>

namespace Camera {

  class FrameV1 {
  public:
    FrameV1() {}
    //  FrameV1 with unassigned contents
    FrameV1(unsigned width, unsigned height, unsigned depth, unsigned offset);
    //  Copy constructor
    FrameV1(const FrameV1&);
  public:
    unsigned width () const;
    unsigned height() const;
    unsigned depth () const;
    unsigned offset() const;

    unsigned depth_bytes() const;
    unsigned data_size  () const;

    const unsigned char* data() const;
    const unsigned char* pixel(unsigned x,unsigned y) const;
  private:
    uint32_t _width;
    uint32_t _height;
    uint32_t _depth;
    uint32_t _offset;
  };

  inline unsigned FrameV1::width () const
  {
    return _width; 
  }

  inline unsigned FrameV1::height() const
  {
    return _height; 
  }

  inline unsigned FrameV1::depth () const
  {
    return _depth; 
  }

  inline unsigned FrameV1::offset() const
  {
    return _offset; 
  }

  inline unsigned FrameV1::depth_bytes() const
  {
    return (_depth+7)>>3;
  }

  inline unsigned FrameV1::data_size() const
  {
    return _width*_height*depth_bytes();
  }

  inline const unsigned char* FrameV1::data() const
  {
    return reinterpret_cast<const unsigned char*>(this+1);
  }

  inline const unsigned char* FrameV1::pixel(unsigned x,
					     unsigned y) const
  {
    return data()+(y*_width+x)*depth_bytes();
  }

};

#endif
