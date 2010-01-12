/*
 * FrameV1.hh
 *
 *  Created on: Nov 6, 2009
 *      Author: jackp
 */

#ifndef PRINCETON_FRAME_V1_HH
#define PRINCETON_FRAME_V1_HH

#include <stdint.h>

#pragma pack(4)

namespace Pds 
{
  
namespace Princeton 
{

class ConfigV1;
class FrameV1 
{
  public:
    static const int Version = 1;

    const uint16_t* data() const;
    
  private:  
    uint32_t _timeStampLo;
    uint32_t _timeStampHi;  
};

} // namespace Princeton

} // namespace Pds 

#pragma pack()

#endif
