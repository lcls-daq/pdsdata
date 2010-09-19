#include "pdsdata/cspad/ElementIterator.hh"

#include "pdsdata/cspad/ConfigV1.hh"
#include "pdsdata/cspad/ConfigV2.hh"
#include "pdsdata/cspad/ElementV1.hh"
#include "pdsdata/cspad/ElementV2.hh"

using namespace Pds::CsPad;

ElementIterator::ElementIterator(const ConfigV1& c, const ElementV1& e) :
  _elem1(&e), _elem2(0), _qmask(c.quadMask()), _amask(c.asicMask())  
{
  for(int iq=0; iq<4; iq++)
    _smask[iq] = _amask==1 ? 0x3 : 0xff;
}

ElementIterator::ElementIterator(const ConfigV2& c, const ElementV1& e) :
  _elem1(&e), _elem2(0), _qmask(c.quadMask()), _amask(c.asicMask())  
{
  for(int iq=0; iq<4; iq++)
    _smask[iq] = c.roiMask(iq);
}

ElementIterator::ElementIterator(const ConfigV2& c, const ElementV2& e) :
  _elem1(0), _elem2(&e) _qmask(c.quadMask()), _amask(c.asicMask())  
{
  for(int iq=0; iq<4; iq++)
    _smask[iq] = c.roiMask(iq);
}

const ElementIterator::ElementV1* next()
{
  _qmask &= ~(1<<_elem1->quad());
  if (_qmask==0)
    return 0;

  const Section*  s = reinterpret_cast<const Section*>(_elem1+1);
  const uint16_t* u = reinterpret_cast<const uint16_t*>(s + (_amask==1 ? 2 : 8));
  _elem1      = reinterpret_cast<const ElementV1*>(u+2);
  _smaskc     = _smask[iq];
  _section    = reinterpret_cast<const Section*>(_elem1+1);
  _section_id = 0;
  return _elem1;
}

const ElementIterator::ElementV2* next()
{
  unsigned iq = _elem2->quad();
  _qmask &= ~(1<<iq);
  if (_qmask==0)
    return 0;

  const Section*  s = reinterpret_cast<const Section*>(_elem2+1);
  const uint16_t* u = reinterpret_cast<const uint16_t*>(s + _scnt[iq]);
  _elem2 = reinterpret_cast<const ElementV2*>(u+2);

  iq = _elem2->quad();
  _smaskc = _smask[iq];
  if (_smaskc) {
    _section    = reinterpret_cast<const Section*>(_elem2+1);
    _section_id = 0; 
    while ((_smaskc&(1<<_section_id))==0)
      _section_id++;
  }
  else
    _section = 0;

  return _elem2;
}

const Section* ElementIterator::next(int sectionID& id)
{
  if (_section==0)
    return 0;

  const Section* s = _section;
  id = _section_id;

  while (++_section_id<8)
    if (_smaskc&(1<<_section_id)) {
      _section++;
      return s;
    }

  _section=0;
  return s;
}
