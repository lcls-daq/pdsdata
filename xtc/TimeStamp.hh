#ifndef PDS_TIMESTAMP_HH
#define PDS_TIMESTAMP_HH

#include <stdint.h>

namespace Pds {
  class TimeStamp {
  public:
    enum {NumFiducialBits = 56};
    enum {MaxFiducials    = (1ULL<<56)-1};
    enum {ErrFiducial     = (1ULL<<56)-1};
  public:
    TimeStamp();
    TimeStamp(const TimeStamp&);
    TimeStamp(const TimeStamp&, unsigned control);
    TimeStamp(uint64_t fiducials, unsigned control=0);

  public:
    uint64_t fiducials() const;  // 929kHz pulse ID
    unsigned control  () const;  // internal bits for alternate interpretation
                                 //   of XTC header fields
  public:
    TimeStamp& operator= (const TimeStamp&);
    bool       operator==(const TimeStamp&) const;
    bool       operator>=(const TimeStamp&) const;
    bool       operator<=(const TimeStamp&) const;
    bool       operator< (const TimeStamp&) const;
    bool       operator> (const TimeStamp&) const;

  private:
    uint64_t _value;
  };
}

#endif
