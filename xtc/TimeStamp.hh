#ifndef PDS_TIMESTAMP_HH
#define PDS_TIMESTAMP_HH

#include <stdint.h>

namespace Pds {
  class TimeStamp {
  public:
    enum {NumFiducialBits = 17};
    enum {MaxFiducials    = (1<<17)-32};
    enum {ErrFiducial     = (1<<17)-1};
    enum {NumPulseIDBits  = 56};
    enum {MaxPulseID      = (1ULL<<56)-1};
    enum {ErrPulseID      = (1ULL<<56)-1};
    enum TimeBase { LCLSI, LCLSII };
  public:
    TimeStamp();
    TimeStamp(const TimeStamp&);
    TimeStamp(const TimeStamp&, unsigned control);
    TimeStamp(unsigned ticks, unsigned fiducials, unsigned vector, unsigned control=0); // LCLS-I
    TimeStamp(uint64_t pulseId, unsigned control=0); // LCLS-II

  public:
    unsigned ticks    () const;  // 119MHz counter within the fiducial for
                                 //   eventcode which initiated the readout
    unsigned fiducials() const;  // 360Hz pulse ID
    unsigned control  () const;  // internal bits for alternate interpretation
                                 //   of XTC header fields
    unsigned vector   () const;  // 15-bit seed for event-level distribution
                                 //   ( events since configure )
    uint64_t pulseID() const;  // 929kHz pulse ID
    TimeBase base   () const;
  public:
    TimeStamp& operator= (const TimeStamp&);
    bool       operator==(const TimeStamp&) const;
    bool       operator>=(const TimeStamp&) const;
    bool       operator<=(const TimeStamp&) const;
    bool       operator< (const TimeStamp&) const;
    bool       operator> (const TimeStamp&) const;

  private:
    uint32_t _low;
    uint32_t _high;
  };
}

#endif
