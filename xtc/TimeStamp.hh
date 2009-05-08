#ifndef PDS_TIMESTAMP_HH
#define PDS_TIMESTAMP_HH

namespace Pds {
  class TimeStamp {
  public:
    enum {NumFiducialBits = 17};

  public:
    TimeStamp();
    TimeStamp(const TimeStamp&);
    TimeStamp(const TimeStamp&, unsigned control);
    TimeStamp(unsigned ticks, unsigned fiducials, unsigned control=0);

  public:
    unsigned ticks()  const;
    unsigned fiducials() const;
    unsigned control() const;

  public:
    TimeStamp& operator= (const TimeStamp&);
    bool       operator==(const TimeStamp&) const;
    bool       operator>=(const TimeStamp&) const;
    bool       operator<=(const TimeStamp&) const;
    bool       operator< (const TimeStamp&) const;
    bool       operator> (const TimeStamp&) const;

  private:
    unsigned _low;
    unsigned _high;
  };
}

#endif
