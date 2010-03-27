#include "pdsdata/ipimb/ConfigV1.hh"
//#include "pdsdata/ipimb/DataDescV1.hh"
#include <stdio.h>

using namespace Pds;

using namespace Ipimb;

static const unsigned Version=1;

Pds::TypeId ConfigV1::typeId() {
  return Pds::TypeId(Pds::TypeId::Id_IpimbConfig,Version);
}

ConfigV1::ConfigV1() {}

ConfigV1::~ConfigV1() {}

void ConfigV1::dump() const {
  printf("------Ipimb Config-------------\n");
  printf("Trigger counter 0x%x\n", (unsigned)triggerCounter());
  printf("Charge range 0x%x\n", chargeAmpRange());
  printf("Diode bias %f\n", diodeBias());
  
}

ConfigV1::ConfigV1 (uint64_t triggerCounter,
		    uint64_t serialID,
		    uint16_t chargeAmpRange,
		    //		    uint16_t calibrationRange,
		    uint32_t resetLength,
		    uint16_t resetDelay,
		    float chargeAmpRefVoltage,
		    //		    float calibrationVoltage,
		    float diodeBias,
		    uint16_t srStatusResets,
		    uint16_t errors,
		    uint16_t calStrobeLength,
		    uint16_t trigDelay) :
  _triggerCounter(triggerCounter),
  _serialID(serialID),
  _chargeAmpRange(chargeAmpRange),
  //  _calibrationRange(calibrationRange),
  _resetLength(resetLength),
  _resetDelay(resetDelay),
  _chargeAmpRefVoltage(chargeAmpRefVoltage),
  //  _calibrationVoltage(calibrationVoltage),
  _diodeBias(diodeBias),
  _srStatusResets(srStatusResets),
  _errors(errors),
  _calStrobeLength(calStrobeLength),
  _trigDelay(trigDelay)
{
}

uint64_t ConfigV1::triggerCounter() const{return _triggerCounter;}
uint64_t ConfigV1::serialID() const{return _serialID;}
uint16_t ConfigV1::chargeAmpRange() const{return _chargeAmpRange;}
//uint16_t ConfigV1::calibrationRange() const{return _calibrationRange;}
uint32_t ConfigV1::resetLength() const{return _resetLength;}
uint16_t ConfigV1::resetDelay() const{return _resetDelay;}
float ConfigV1::chargeAmpRefVoltage() const{return _chargeAmpRefVoltage;}
//float ConfigV1::calibrationVoltage() const{return _calibrationVoltage;}
float ConfigV1::diodeBias() const{return _diodeBias;}
uint16_t ConfigV1::srStatusResets() const{return _srStatusResets;}
uint16_t ConfigV1::errors() const{return _errors;}
uint16_t ConfigV1::calStrobeLength() const{return _calStrobeLength;}
uint16_t ConfigV1::trigDelay() const{return _trigDelay;}

