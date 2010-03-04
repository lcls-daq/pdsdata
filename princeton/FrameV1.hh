/*
 * FrameV1.hh
 *
 *  Created on: Nov 6, 2009
 *      Author: jackp
 */

#ifndef PRINCETON_FRAME_V1_HH
#define PRINCETON_FRAME_V1_HH

#include <stdio.h>
#include <stdint.h>
#include <stdexcept>

#pragma pack(4)

namespace Pds 
{
  
namespace Princeton 
{

class ConfigV1;

/**
 * class FrameV1
 *
 * Usage:
 *   Before the object constructor FrameV1() is called, user need to call initStatic() to initialize the associated config object
 *   fot the future frame objects. Because some important attributes, such as image width, height and binning factors are stored
 *   in the config object.
 */
class FrameV1 
{
public:
  static const int Version = 1;

  FrameV1( uint32_t  iShotIdStart, float fReadoutTime );    
  const uint16_t*   data()        const;
  
  uint32_t          shotIdStart() const { return _iShotIdStart; }
  float             readoutTime() const { return _fReadoutTime; }  
  
private:  
  uint32_t  _iShotIdStart;
  float     _fReadoutTime;
};


} // namespace Princeton

} // namespace Pds 

#pragma pack()

#endif
