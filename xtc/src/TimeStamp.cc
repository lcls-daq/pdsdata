#include "pdsdata/xtc/TimeStamp.hh"

namespace Pds {
  enum {v_ticks =  0, k_ticks = 24};
  enum {v_cntrl = 24, k_cntrl = 8};
  enum {m_ticks = ((1 << k_ticks)-1), s_ticks = (m_ticks << v_ticks)};
  enum {m_cntrl = ((1 << k_cntrl)-1), s_cntrl = (m_cntrl << v_cntrl)};
}
 
Pds::TimeStamp::TimeStamp() :
  _low(0),
  _high(0)
{}

Pds::TimeStamp::TimeStamp(const Pds::TimeStamp& input) :
  _low(input._low),
  _high(input._high)
{}

Pds::TimeStamp::TimeStamp(const Pds::TimeStamp& input, unsigned control) :
  _low((input._low & s_ticks) | ((control & m_cntrl) << v_cntrl)),
  _high(input._high)
{}

Pds::TimeStamp::TimeStamp(unsigned low, unsigned high, unsigned control) :
  _low((low & s_ticks) | ((control & m_cntrl) << v_cntrl)),
  _high(high)
{}

unsigned Pds::TimeStamp::ticks() const
{
  return (_low & s_ticks) >> v_ticks;
}

unsigned Pds::TimeStamp::fiducials() const
{
  return _high;
}

unsigned Pds::TimeStamp::control() const
{
  return (_low & s_cntrl) >> v_cntrl;
}

Pds::TimeStamp& Pds::TimeStamp::operator=(const Pds::TimeStamp& input)
{
  _low = input._low;
  _high = input._high;
  return *this;
}

bool Pds::TimeStamp::operator==(const Pds::TimeStamp& ref) const
{
  return fiducials() == ref.fiducials();
}

bool Pds::TimeStamp::operator>=(const Pds::TimeStamp& ref) const
{
  return fiducials() >= ref.fiducials();
}

bool Pds::TimeStamp::operator<=(const Pds::TimeStamp& ref) const
{
  return fiducials() <= ref.fiducials();
}

bool Pds::TimeStamp::operator>(const Pds::TimeStamp& ref) const
{
  return fiducials() > ref.fiducials();
}

bool Pds::TimeStamp::operator<(const Pds::TimeStamp& ref) const
{
  return fiducials() < ref.fiducials();
}
