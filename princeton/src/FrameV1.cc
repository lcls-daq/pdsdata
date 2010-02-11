#include "pdsdata/princeton/FrameV1.hh"
#include "pdsdata/princeton/ConfigV1.hh"

using namespace Pds;
using namespace Princeton;

FrameV1::FrameV1( uint32_t iShotIdStart, uint32_t iShotIdEnd, float fReadoutTime ) :
 _iShotIdStart(iShotIdStart), _iShotIdEnd(iShotIdEnd), _fReadoutTime(fReadoutTime)
{
  // Check if initStatic() was called before this function
  if ( _pConfig == NULL )
    throw( FrameV1Exception( "FrameV1::FrameV1(): initStatic() need to be called to initialize the ConfigV1 object\n" ) );
}

const uint16_t* FrameV1::data() const
{
  return (uint16_t*) (this + 1);
}

/*
 * public static functions
 */
void FrameV1::initStatic( ConfigV1& config )
{
  _pConfig = &config;
}

int FrameV1::size()
{
  if ( _pConfig == NULL )
    throw( FrameV1Exception( "FrameV1::size(): initStatic() need to be called to initialize the ConfigV1 object\n" ) );
    
  //return sizeof(FrameV1) + _pConfig->width() * _pConfig->height() * 2; // 2 -> 16 bit color depth
  return sizeof(FrameV1) + 4*1024*1024*2; // 2 -> 16 bit color depth // !! debug
  //return sizeof(FrameV1) + 0; // 2 -> 16 bit color depth // !! debug
}

/*
 * Definition of static private data
 */
ConfigV1* FrameV1::_pConfig = NULL;
