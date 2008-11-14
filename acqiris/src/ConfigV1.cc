
#include "pdsdata/acqiris/ConfigV1.hh"

using namespace Pds;

using namespace Acqiris;

ConfigV1::ConfigV1() {}

ConfigV1::ConfigV1 (double   sampInterval,
                    double   delayTime,   
                    uint32_t nbrSamples,  
                    uint32_t nbrSegments, 
                    uint32_t coupling,    
                    uint32_t bandwidth,   
                    double   fullScale,   
                    double   offset,      
                    uint32_t trigCoupling,
                    uint32_t trigInput,   
                    uint32_t trigSlope,   
                    double   trigLevel) :
  _sampInterval(sampInterval),
  _delayTime   (delayTime),   
  _nbrSamples  (nbrSamples),  
  _nbrSegments (nbrSegments), 
  _coupling    (coupling),    
  _bandwidth   (bandwidth),   
  _fullScale   (fullScale),   
  _offset      (offset),      
  _trigCoupling(trigCoupling),
  _trigInput   (trigInput),   
  _trigSlope   (trigSlope),   
  _trigLevel   (trigLevel) {}

static const unsigned Version=1;

Pds::TypeId ConfigV1::typeId() {
  return Pds::TypeId(Pds::TypeId::Id_AcqConfig,Version);
}

double   ConfigV1::sampInterval() const {return _sampInterval;}
double   ConfigV1::delayTime()    const {return _delayTime;}
uint32_t ConfigV1::nbrSamples()   const {return _nbrSamples;}
uint32_t ConfigV1::nbrSegments()  const {return _nbrSegments;}
uint32_t ConfigV1::coupling()     const {return _coupling;}
uint32_t ConfigV1::bandwidth()    const {return _bandwidth;}
double   ConfigV1::fullScale()    const {return _fullScale;}
double   ConfigV1::offset()       const {return _offset;}
uint32_t ConfigV1::trigCoupling() const {return _trigCoupling;}
uint32_t ConfigV1::trigInput()    const {return _trigInput;}
uint32_t ConfigV1::trigSlope()    const {return _trigSlope;}
double   ConfigV1::trigLevel()    const {return _trigLevel;}
