
#include "pdsdata/acqiris/ConfigV1.hh"

using namespace Pds;

using namespace Acqiris;

static const unsigned Version=1;

Pds::TypeId ConfigV1::typeId() {
  return Pds::TypeId(Pds::TypeId::Id_AcqConfig,Version);
}

ConfigV1::ConfigV1() {}

HorizConfigV1::HorizConfigV1() {}

HorizConfigV1::HorizConfigV1(double   sampInterval,
                            double   delayTime,   
                            uint32_t nbrSamples,  
                            uint32_t nbrSegments) :
  _sampInterval(sampInterval),
  _delayTime   (delayTime),   
  _nbrSamples  (nbrSamples),  
  _nbrSegments (nbrSegments)
{}

double   HorizConfigV1::sampInterval() const {return _sampInterval;}
double   HorizConfigV1::delayTime()    const {return _delayTime;}
uint32_t HorizConfigV1::nbrSamples()   const {return _nbrSamples;}
uint32_t HorizConfigV1::nbrSegments()  const {return _nbrSegments;}

TrigConfigV1::TrigConfigV1() {}

TrigConfigV1::TrigConfigV1(uint32_t trigCoupling,
                           uint32_t trigInput,   
                           uint32_t trigSlope,   
                           double   trigLevel) :
  _trigCoupling(trigCoupling),
  _trigInput   (trigInput),   
  _trigSlope   (trigSlope),   
  _trigLevel   (trigLevel)
{}

uint32_t TrigConfigV1::trigCoupling() const {return _trigCoupling;}
uint32_t TrigConfigV1::trigInput()    const {return _trigInput;}
uint32_t TrigConfigV1::trigSlope()    const {return _trigSlope;}
double   TrigConfigV1::trigLevel()    const {return _trigLevel;}

VertConfigV1::VertConfigV1() {}
VertConfigV1::VertConfigV1(double   fullScale,
                           double   offset,
                           uint32_t coupling,    
                           uint32_t bandwidth) :
  _fullScale(fullScale),  _offset(offset),
  _coupling(coupling), _bandwidth(bandwidth) {}

ConfigV1::ConfigV1 (uint32_t nbrConvertersPerChannel,
                    uint32_t channelMask,
                    uint32_t nbrBanks,
                    const TrigConfigV1& trigConfig,
                    const HorizConfigV1& horizConfig,
                    const VertConfigV1* vertConfig) :
  _nbrConvertersPerChannel(nbrConvertersPerChannel),
  _channelMask(channelMask),_trigConfig(trigConfig),
  _horizConfig(horizConfig) {
  for (unsigned i=0;i<MaxChan;i++) {
    _vertConfig[i]=vertConfig[i];
  }
}

double   VertConfigV1::fullScale() const {return _fullScale;}
double   VertConfigV1::offset()    const {return _offset;}
uint32_t VertConfigV1::coupling()  const {return _coupling;}
uint32_t VertConfigV1::bandwidth() const {return _bandwidth;}

uint32_t ConfigV1::nbrConvertersPerChannel() const {return _nbrConvertersPerChannel;}

uint32_t ConfigV1::channelMask() const {return _channelMask;}

uint32_t ConfigV1::nbrChannels() const {
  uint32_t numchan=0;
  for (unsigned i=0;i<32;i++) if (_channelMask&(1<<i)) numchan++;
  return numchan;
}

uint32_t ConfigV1::nbrBanks() const {return _nbrBanks;}

HorizConfigV1& ConfigV1::horizConfig() {return _horizConfig;}

TrigConfigV1& ConfigV1::trigConfig() {return _trigConfig;}

VertConfigV1& ConfigV1::vertConfig(uint32_t channel) {return _vertConfig[channel];}

const HorizConfigV1& ConfigV1::horizConfig() const {return _horizConfig;}

const TrigConfigV1& ConfigV1::trigConfig() const {return _trigConfig;}

const VertConfigV1& ConfigV1::vertConfig(uint32_t channel) const {return _vertConfig[channel];}
