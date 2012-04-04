#ifndef OCEANOPTICS_FRAME_V1_HH
#define OCEANOPTICS_FRAME_V1_HH

#include <stdio.h>
#include <stdint.h>
#include <stdexcept>

#include "ConfigV1.hh"

#pragma pack(4)

namespace Pds 
{
  
namespace OceanOptics 
{
  
class DataV1 
{
public:
  static const int Version            = 1;
  static const int iDataReadSize      = 8192;
  static const int iNumPixels         = 3840;
  static const int iActivePixelIndex  = 22;

  DataV1() {}
  
  uint16_t*         data                () {return (uint16_t*) this;}
  uint64_t          frameCounter        () {return _u64FrameCounter;}
  uint64_t          numDelayedFrames    () {return _u64NumDelayedFrames;}
  uint64_t          numDiscardFrames    () {return _u64NumDiscardFrames;}
  struct timespec   timeFrameStart      () {return _tsTimeFrameStart;}
  struct timespec   timeFrameFirstData  () {return _tsTimeFrameFirstData;}
  struct timespec   timeFrameEnd        () {return _tsTimeFrameEnd;}
  int8_t            numSpectraInData    () {return _i8NumSpectraInData;}
  int8_t            numSpectraInQueue   () {return _i8NumSpectraInQueue;}
  int8_t            numSpectraUnused    () {return _i8NumSpectraUnused;}
  double            durationOfFrame     (); // return unit: in seconds
  
  double            nonlinerCorrected   (const ConfigV1& c, int iPixel);  
  
  static double     waveLength          (const ConfigV1& c, int iPixel);
  static double     waveLength1stOrder  (const ConfigV1& c, int iPixel);
  
private:  
  uint16_t          lu16Spetra[iNumPixels];
  
  /*
   * Spectra information: stored after the pixel data
   */
  uint64_t          _u64FrameCounter;    // count from 0
  uint64_t          _u64NumDelayedFrames;
  uint64_t          _u64NumDiscardFrames;
  struct timespec   _tsTimeFrameStart;
  struct timespec   _tsTimeFrameFirstData;
  struct timespec   _tsTimeFrameEnd;
  int32_t           _i32Version;
  int8_t            _i8NumSpectraInData;
  int8_t            _i8NumSpectraInQueue;
  int8_t            _i8NumSpectraUnused;
  int8_t            _iReserved1;  
};


} // namespace OceanOptics

} // namespace Pds 

#pragma pack()

#endif
