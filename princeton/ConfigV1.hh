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
  static const int Version                        = 1;
  static const unsigned int uPI_MTE_1300_Width    = 1340;
  static const unsigned int uPI_MTE_1300_Height   = 1300;
  //static const unsigned int uPI_MTE_2048_Width    = 2048;
  //static const unsigned int uPI_MTE_2048_Height   = 2048;
  static const unsigned int uPI_MTE_2048_Width    = 512; // !! for debug only
  static const unsigned int uPI_MTE_2048_Height   = 512;
  
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
  
  ConfigV1(uint32_t uWidth = uPI_MTE_2048_Width, uint32_t uHeight = uPI_MTE_2048_Height, 
   uint32_t uOrgX = 0, uint32_t uOrgY = 0, uint32_t uBinX = 1, uint32_t uBinY = 1,
   int16_t  iCoolingTemp = -1000, EnumExposureMode iExposureMode = STROBED_MODE, 
   float fExposureTime = 0.005 );

  uint32_t          width()         { return _uWidth; }
  uint32_t          height()        { return _uHeight; }
  uint32_t          orgX()          { return _uOrgX; }
  uint32_t          orgY()          { return _uOrgY; }    
  uint32_t          binX()          { return _uBinX; }
  uint32_t          binY()          { return _uBinY; }    
  int16_t           coolingTemp()   { return _iCoolingTemp; }
  EnumExposureMode  exposureMode()  { return (EnumExposureMode) _iExposureMode; }
  float             exposureTime()  { return _fExposureTime; }
  
private:
  uint32_t          _uWidth, _uHeight;
  uint32_t          _uOrgX,  _uOrgY;
  uint32_t          _uBinX,  _uBinY;
  int16_t           _iCoolingTemp;
  int16_t           _iExposureMode;
  float             _fExposureTime;  
};

} // namespace Princeton

} // namespace Pds 

#pragma pack()

#endif
