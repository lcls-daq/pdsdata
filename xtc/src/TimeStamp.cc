#include "pdsdata/xtc/TimeStamp.hh"

/* bit field access enums
*       v is the index of the rightmost bit
*       k is the number bits in the field
*       m is the mask, right justified
*       s is the mask shifted into place
*/

namespace Pds {
  enum {v_fiduc =  0, k_fiduc = 56 };
  enum {v_cntrl = 56, k_cntrl = 8};
  static const uint64_t m_fiduc = ((1ULL << k_fiduc)-1);
  static const uint64_t s_fiduc = (m_fiduc << v_fiduc);
  static const uint64_t m_cntrl = ((1ULL << k_cntrl)-1);
  static const uint64_t s_cntrl = (m_cntrl << v_cntrl);
}
 
Pds::TimeStamp::TimeStamp() :
  _value (0)
{}

Pds::TimeStamp::TimeStamp(const Pds::TimeStamp& input) :
  _value (input._value)
{}

Pds::TimeStamp::TimeStamp(const Pds::TimeStamp& input, unsigned control) :
  _value ((input._value & s_fiduc) | ((uint64_t(control) & m_cntrl) << v_cntrl))
{}

Pds::TimeStamp::TimeStamp(uint64_t fiducials, unsigned control) :
  _value ((fiducials & s_fiduc) | ((uint64_t(control) & m_cntrl) << v_cntrl))
{}

uint64_t Pds::TimeStamp::fiducials() const
{
  return (_value & s_fiduc) >> v_fiduc;
}

unsigned Pds::TimeStamp::control() const
{
  return (_value & s_cntrl) >> v_cntrl;
}

Pds::TimeStamp& Pds::TimeStamp::operator=(const Pds::TimeStamp& input)
{
  _value  = input._value ;
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
