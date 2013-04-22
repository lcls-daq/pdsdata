#include "pdsdata/imp/ConfigV1.hh"
#include <stdio.h>
#include <string.h>

using namespace Pds::Imp;

class RegisterV1 {
  public:
    RegisterV1() {};
    ~RegisterV1() {};
    uint32_t mask;
    uint32_t defaultValue;
};

static uint32_t _regsfoo[ConfigV1::NumberOfRegisters][2] = {
    //addr     mask           default
    { 0xffff,        0x1111    },   //    Range
    { 0xffff,        0x5555    },   //    Cal_range
    { 0xffffffff,    0x98968200},   //    Reset
    { 0xffff,        0x1999    },   //    Bias_data
    { 0xffff,        0x2fb5    },   //    Cal_data
    { 0xffff,        0x3fff    },   //    BiasDac_data
    { 0xffff,        0x927c    },   //    Cal_strobe
    { 0x3ff,         0x3ff     },   //    NumberOfSamples
    { 0xffff,        0x100     },   //    TrigDelay
    { 0xffff,        0x1f4     }    //    Adc_delay
};

static RegisterV1* _regs = (RegisterV1*) _regsfoo;

static bool           namesAreInitialized = false;

ConfigV1::ConfigV1() {
  if (!namesAreInitialized){
    int r;
    for (r=0; r< (int)ConfigV1::NumberOfRegisters; r++) {
      name((ConfigV1::Registers)r, true);
    }
    namesAreInitialized = true;
  }
}

ConfigV1::ConfigV1(
    unsigned a,
    unsigned b,
    unsigned c,
    unsigned d,
    unsigned e,
    unsigned f,
    unsigned g,
    unsigned h,
    unsigned i,
    unsigned j) {
  _values[0] = a;
  _values[1] = b;
  _values[2] = c;
  _values[3] = d;
  _values[4] = e;
  _values[5] = f;
  _values[6] = g;
  _values[7] = h;
  _values[8] = i;
  _values[9] = j;
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
  return ((_values[r]) & _regs[r].mask);
}

const uint32_t ConfigV1::get (Registers r) const {
  if (r >= ConfigV1::NumberOfRegisters) {
    printf("ConfigV1::get parameter out of range!! %u\n", r);
    return 0;
  }
  return ((_values[r]) & _regs[r].mask);
}

uint32_t   ConfigV1::set (Registers r, uint32_t v) {
  if (r >= ConfigV1::NumberOfRegisters) {
    printf("ConfigV1::set parameter out of range!! %u\n", r);
    return 0;
  }
  _values[r] = _regs[r].mask & v;
  return 0;
}

uint32_t   ConfigV1::rangeHigh(Registers r) {
  if (r >= ConfigV1::NumberOfRegisters) {
    printf("ConfigV1::rangeHigh parameter out of range!! %u\n", r);
    return 0;
  }
  return _regs[r].mask;
}

uint32_t   ConfigV1::rangeLow(Registers r) {
  if (r==ConfigV1::NumberOfSamples) {
    return 1;
  }
  return 0;
}

uint32_t   ConfigV1::defaultValue(Registers r) {
  if (r >= ConfigV1::NumberOfRegisters) {
    printf("ConfigV1::defaultValue parameter out of range!! %u\n", r);
    return 0;
  }
  return _regs[r].defaultValue & _regs[r].mask;
 }

char*   const   ConfigV1::name     (Registers r, bool init) {
  static char _regsNames[NumberOfRegisters+1][120] = {
      {"Range"},              //      Range
      {"Cal_range"},          //      Cal_range
      {"Reset"},              //      Reset
      {"Bias_data"},          //      Bias_data
      {"Cal_data"},           //      Cal_data
      {"BiasDac_data"},       //      BiasDac_data
      {"Cal_strobe"},         //      Cal_strobe
      {"NumberOfSamples"},    //      NumberOfSamples
      {"TrigDelay"},          //    TrigDelay
      {"AdcDelay"},           //      AdcDelay
    {"----INVALID----"}       //      NumberOfRegisters
  };
  static char range[60];

  if (init && (r < ConfigV1::NumberOfRegisters)) {
    sprintf(range, "  (%u..%u)    ", 0, _regs[r].mask);
    strncat(_regsNames[r], range, 40);
  }

  return r < ConfigV1::NumberOfRegisters ? _regsNames[r] : _regsNames[ConfigV1::NumberOfRegisters];

};

