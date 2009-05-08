#ifndef Pds_ClockTime_hh
#define Pds_ClockTime_hh

namespace Pds {
  class ClockTime {
  public:
    ClockTime();
    ClockTime(const ClockTime& t);
    ClockTime(unsigned sec, unsigned nsec);

  public:
    unsigned seconds    () const {return _high;}
    unsigned nanoseconds() const {return _low;}

  public:
    ClockTime& operator=(const ClockTime&);
    bool operator>(const ClockTime&) const; 

  private:
    unsigned _low;
    unsigned _high;
  };
}
#endif
