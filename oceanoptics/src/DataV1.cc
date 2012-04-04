#include "pdsdata/oceanoptics/DataV1.hh"

namespace Pds 
{
  
namespace OceanOptics 
{

double DataV1::durationOfFrame()
{
  return _tsTimeFrameEnd.tv_sec - _tsTimeFrameStart.tv_sec + ( _tsTimeFrameEnd.tv_nsec - _tsTimeFrameStart.tv_nsec) * 1e-9;
}

double DataV1::waveLength(const ConfigV1& c, int iPixel)
{
  return 
     c.waveLenCalib(0) + iPixel * 
    (c.waveLenCalib(1) + iPixel * 
    (c.waveLenCalib(2) + iPixel * 
     c.waveLenCalib(3)));
}

double DataV1::waveLength1stOrder  (const ConfigV1& c, int iPixel)
{
  return c.waveLenCalib(0) + iPixel * c.waveLenCalib(1);  
}

double DataV1::nonlinerCorrected   (const ConfigV1& c, int iPixel)
{
  double fRawValue = (double) (lu16Spetra[iPixel] ^ 0x2000);
  return fRawValue / (
     c.nonlinCorrect(0) + fRawValue * 
    (c.nonlinCorrect(1) + fRawValue * 
    (c.nonlinCorrect(2) + fRawValue * 
    (c.nonlinCorrect(3) + fRawValue * 
    (c.nonlinCorrect(4) + fRawValue * 
    (c.nonlinCorrect(5) + fRawValue * 
    (c.nonlinCorrect(6) + fRawValue * 
     c.nonlinCorrect(7))))))));
}

} // namespace OceanOptics

} // namespace Pds 
