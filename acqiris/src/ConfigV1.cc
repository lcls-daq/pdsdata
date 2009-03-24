
#include "pdsdata/acqiris/ConfigV1.hh"

using namespace Pds;

using namespace Acqiris;

static const unsigned Version=1;

Pds::TypeId ConfigV1::typeId() {
  return Pds::TypeId(Pds::TypeId::Id_AcqConfig,Version);
}

ConfigV1::ConfigV1() {}

HorizV1::HorizV1() {}

HorizV1::HorizV1(double   sampInterval,
                 double   delayTime,   
                 uint32_t nbrSamples,  
                 uint32_t nbrSegments) :
  _sampInterval(sampInterval),
  _delayTime   (delayTime),   
  _nbrSamples  (nbrSamples),  
  _nbrSegments (nbrSegments)
{}

double   HorizV1::sampInterval() const {return _sampInterval;}
double   HorizV1::delayTime()    const {return _delayTime;}
uint32_t HorizV1::nbrSamples()   const {return _nbrSamples;}
uint32_t HorizV1::nbrSegments()  const {return _nbrSegments;}

TrigV1::TrigV1() {}

TrigV1::TrigV1(uint32_t trigCoupling,
               uint32_t trigInput,   
               uint32_t trigSlope,   
               double   trigLevel) :
  _trigCoupling(trigCoupling),
  _trigInput   (trigInput),   
  _trigSlope   (trigSlope),   
  _trigLevel   (trigLevel)
{}

uint32_t TrigV1::trigCoupling() const {return _trigCoupling;}
uint32_t TrigV1::trigInput()    const {return _trigInput;}
uint32_t TrigV1::trigSlope()    const {return _trigSlope;}
double   TrigV1::trigLevel()    const {return _trigLevel;}

VertV1::VertV1() {}
VertV1::VertV1(double   fullScale,
               double   offset,
               uint32_t coupling,    
               uint32_t bandwidth) :
  _fullScale(fullScale),  _offset(offset),
  _coupling(coupling), _bandwidth(bandwidth) {}

ConfigV1::ConfigV1 (uint32_t nbrConvertersPerChannel,
                    uint32_t channelMask,
                    uint32_t nbrBanks,
                    const TrigV1& trig,
                    const HorizV1& horiz,
                    const VertV1* vert) :
  _nbrConvertersPerChannel(nbrConvertersPerChannel),
  _channelMask(channelMask),_trig(trig),
  _horiz(horiz) {
  for (unsigned i=0;i<MaxChan;i++) {
    _vert[i]=vert[i];
  }
}

double   VertV1::fullScale() const {return _fullScale;}
double   VertV1::offset()    const {return _offset;}
uint32_t VertV1::coupling()  const {return _coupling;}
uint32_t VertV1::bandwidth() const {return _bandwidth;}

uint32_t ConfigV1::nbrConvertersPerChannel() const {return _nbrConvertersPerChannel;}

uint32_t ConfigV1::channelMask() const {return _channelMask;}

uint32_t ConfigV1::nbrChannels() const {
  uint32_t numchan=0;
  for (unsigned i=0;i<32;i++) if (_channelMask&(1<<i)) numchan++;
  return numchan;
}

uint32_t ConfigV1::nbrBanks() const {return _nbrBanks;}

HorizV1& ConfigV1::horiz() {return _horiz;}

TrigV1& ConfigV1::trig() {return _trig;}

VertV1& ConfigV1::vert(uint32_t channel) {return _vert[channel];}

const HorizV1& ConfigV1::horiz() const {return _horiz;}

const TrigV1& ConfigV1::trig() const {return _trig;}

const VertV1& ConfigV1::vert(uint32_t channel) const {return _vert[channel];}
