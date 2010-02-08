#include "pdsdata/princeton/ConfigV1.hh"

#include <string.h>

using namespace Pds;
using namespace Princeton;

ConfigV1::ConfigV1(uint32_t uWidth, uint32_t uHeight, uint32_t uOrgX, uint32_t uOrgY, uint32_t uBinX, uint32_t uBinY,
   int16_t i16CoolingTemp, EnumExposureMode enumExposureMode, float f32ExposureTime, uint8_t u8MakeUpEventMode) :
 _uWidth(uWidth), _uHeight(uHeight), _uOrgX(uOrgX), _uOrgY(uOrgY), _uBinX(uBinX), _uBinY(uBinY),
 _i16CoolingTemp(i16CoolingTemp), _i16ExposureMode((int16_t)enumExposureMode), _f32ExposureTime(f32ExposureTime),
 _u8MakeUpEventMode(u8MakeUpEventMode)
 {}
