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
  static const int Version = 1;
  static const int iPI_MTE_1300_Width   = 1340;
  static const int iPI_MTE_1300_Height  = 1300;
  static const int iPI_MTE_2048_Width   = 2048;
  static const int iPI_MTE_2048_Height  = 2048;
  
  ConfigV1(uint32_t uWidth = iPI_MTE_2048_Width, uint32_t uHeight = iPI_MTE_2048_Height, 
   uint32_t uOrgX = 0, uint32_t uOrgY = 0, 
   uint32_t uBinX = 1, uint32_t uBinY = 1 );

  int width()   { return _uWidth; }
  int height()  { return _uHeight; }
  int orgX()    { return _uOrgX; }
  int orgY()    { return _uOrgY; }    
  int binX()    { return _uBinX; }
  int binY()    { return _uBinY; }    
  
private:
  uint32_t _uWidth, _uHeight;
  uint32_t _uOrgX,  _uOrgY;
  uint32_t _uBinX,  _uBinY;
};

} // namespace Princeton

} // namespace Pds 

#pragma pack()

#endif
