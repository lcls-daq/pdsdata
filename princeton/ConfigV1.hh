#ifndef PRINCETON_CONFIG_V1_HH
#define PRINCETON_CONFIG_V1_HH

#include <stdint.h>

#pragma pack(4)

namespace Pds 
{

namespace Princeton 
{

class ConfigV1 
{
public:
  static const int Version                              = 1;
  static const unsigned int uPI_MTE_1300_Width          = 1340;
  static const unsigned int uPI_MTE_1300_Height         = 1300;
  //static const unsigned int uPI_MTE_2048_Width    = 2048;
  //static const unsigned int uPI_MTE_2048_Height   = 2048;
  static const unsigned int uPI_MTE_2048_Width          = 512; // !! for debug only
  static const unsigned int uPI_MTE_2048_Height         = 512;
  static const int          iPI_MTE_2048_MaxSpeedIndex  = 5;
  
  enum EnumExposureMode // copied from pvcam.h
  { 
    TIMED_MODE, 
    STROBED_MODE, 
    BULB_MODE, 
    TRIGGER_FIRST_MODE, 
    FLASH_MODE,
    VARIABLE_TIMED_MODE, 
    INT_STROBE_MODE
  };  
  
  ConfigV1(
   uint32_t         uWidth                = uPI_MTE_2048_Width, 
   uint32_t         uHeight               = uPI_MTE_2048_Height, 
   uint32_t         uOrgX                 = 0, 
   uint32_t         uOrgY                 = 0, 
   uint32_t         uBinX                 = 1, 
   uint32_t         uBinY                 = 1,
   int16_t          iCoolingTemp          = -1000, 
   EnumExposureMode iExposureMode         = STROBED_MODE, 
   float            f32ExposureTime       = 0.005, 
   int16_t          i16ReadoutSpeedIndex  = iPI_MTE_2048_MaxSpeedIndex,
   uint8_t          uDelayMode            = 0 );

  uint32_t          width()             { return _uWidth; }
  uint32_t          height()            { return _uHeight; }
  uint32_t          orgX()              { return _uOrgX; }
  uint32_t          orgY()              { return _uOrgY; }    
  uint32_t          binX()              { return _uBinX; }
  uint32_t          binY()              { return _uBinY; }    
  int16_t           coolingTemp()       { return _i16CoolingTemp; }
  EnumExposureMode  exposureMode()      { return (EnumExposureMode) _i16ExposureMode; }
  float             exposureTime()      { return _f32ExposureTime; }
  int16_t           readoutSpeedIndex() { return _i16ReadoutSpeedIndex; }
  uint8_t           makeUpEventMode()   { return _u8DelayMode; }

  uint32_t          setWidth(uint32_t uWidth)     { return _uWidth = uWidth; }
  uint32_t          setHeight(uint32_t uHeight)   { return _uHeight = uHeight; }
  
  int               frameSize();  // calculate the frame size based on the current ROI and binning settings
  
private:
  uint32_t          _uWidth, _uHeight;
  uint32_t          _uOrgX,  _uOrgY;
  uint32_t          _uBinX,  _uBinY;
  int16_t           _i16CoolingTemp;
  int16_t           _i16ExposureMode;
  float             _f32ExposureTime;
  int16_t           _i16ReadoutSpeedIndex;
  uint8_t           _u8DelayMode;
};

} // namespace Princeton

} // namespace Pds 

#pragma pack()

#endif
