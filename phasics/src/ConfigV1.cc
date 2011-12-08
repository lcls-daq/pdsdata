#include "pdsdata/phasics/ConfigV1.hh"
#include <stdio.h>
#include <string.h>


namespace Pds
{
  namespace Phasics
  {
    class RegisterV1 {
      public:
        RegisterV1() {};
        ~RegisterV1() {};
        uint32_t max;
        uint32_t defaultValue;
    };

    static uint32_t _regsfoo[ConfigV1::NumberOfRegisters][2] = {
        //max     default value
        { 1023,   490 },       // Brightness
        { 511,    255 },       // Gain
        { 1016,   508 }        // Sharpness
    };

    static RegisterV1* _regs = (RegisterV1*) _regsfoo;

    static bool           namesAreInitialized = false;
  }
}

using namespace Pds::Phasics;

ConfigV1::ConfigV1() {
  if (!namesAreInitialized){
    int r;
    for (r=0; r< (int)ConfigV1::NumberOfRegisters; r++) {
      name((ConfigV1::Registers)r, true);
    }
    namesAreInitialized = true;
  }
}

uint32_t   ConfigV1::get (Registers r) {
  if (r >= ConfigV1::NumberOfRegisters) {
    printf("ConfigV1::get parameter out of range!! %u\n", r);
    return 0;
  }
  return (_values[r]);
}

const uint32_t ConfigV1::get (Registers r) const {
  if (r >= ConfigV1::NumberOfRegisters) {
    printf("ConfigV1::get parameter out of range!! %u\n", r);
    return 0;
  }
  return (_values[r]);
}

uint32_t   ConfigV1::set (Registers r, uint32_t v) {
  if (r >= ConfigV1::NumberOfRegisters) {
    printf("ConfigV1::set parameter out of range!! %u\n", r);
    return 0;
  }
  if (v > _regs[r].max) _values[r] = _regs[r].max;
  else _values[r] = v;
  return 0;
}

uint32_t   ConfigV1::rangeHigh(Registers r) {
  if (r >= ConfigV1::NumberOfRegisters) {
    printf("ConfigV1::rangeHigh parameter out of range!! %u\n", r);
    return 0;
  }
  return _regs[r].max;
}

uint32_t   ConfigV1::rangeLow(Registers r) {
  return 0;
}

uint32_t   ConfigV1::defaultValue(Registers r) {
  if (r >= ConfigV1::NumberOfRegisters) {
    printf("ConfigV1::defaultValue parameter out of range!! %u\n", r);
    return 0;
  }
  return _regs[r].defaultValue;
 }

char*               ConfigV1::name     (Registers r, bool init) {
  static char _regsNames[NumberOfRegisters+1][120] = {
      {"Brightness"},      //      Brightness
      {"Gain"},            //      Gain
      {"Sharpness"},       //      Sharpness
      {"----INVALID----"}  //      NumberOfRegisters
  };
  static char range[60];

  if (init && (r < ConfigV1::NumberOfRegisters)) {
    sprintf(range, "  (%u..%u)    ", 0, _regs[r].max);
    strncat(_regsNames[r], range, 40);
  }

  return r < ConfigV1::NumberOfRegisters ? _regsNames[r] : _regsNames[ConfigV1::NumberOfRegisters];

};

