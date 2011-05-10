#include "pdsdata/xtc/DetInfo.hh"
#include "pdsdata/xtc/ProcInfo.hh"
#include "pdsdata/xtc/BldInfo.hh"

#include "pdsdata/index/XtcIterL1Accept.hh"

namespace Pds
{  
namespace Index 
{

int XtcIterL1Accept::process(Xtc * xtc)
{
  Level::Type     level             = xtc->src.level();
  long long       lliOffsetPayload  = _lliOffset + sizeof(Xtc);
  bool            bStopUpdate       = false;
  
  if (level == Level::Segment)
  {
    if ( _depth != 0 )
      printf( "XtcIterL1Accept::process(): *** Error depth: Expect 0, but get %d, level %s\n", 
        _depth, Level::name(level) );
    
    _pIndexList->updateSegment( *xtc );    
  }  
  else if (level == Level::Source)
  {
    if ( _depth != 1 )
      printf( "XtcIterL1Accept::process(): *** Error depth: Expect 1, but get %d, level %s\n", 
        _depth, Level::name(level) );
     
    
    _pIndexList->updateSource( *xtc, bStopUpdate );    
  }  
  else if (level == Level::Reporter)
  {
    if ( _depth != 1 )
      printf( "XtcIterL1Accept::process(): *** Error depth: Expect 1, but get %d, level %s\n", 
        _depth, Level::name(level) );
    
    _pIndexList->updateReporter( *xtc, bStopUpdate );    
  }
  else
  {         
    printf( "XtcIterL1Accept::process(): *** Error level %s depth = %d", Level::name(level), _depth );     
  }  

  _lliOffset += sizeof(Xtc) + xtc->sizeofPayload();
    
  // depth > 0 : Will stop after current node
  
  if ( bStopUpdate )
    return XtcIterL1Accept::Stop;
    
  // Remaining case: depth == 0 : Will process more segment nodes
  
  if (xtc->contains.id() == TypeId::Id_Xtc)
  {
    XtcIterL1Accept iter(xtc, _depth + 1, lliOffsetPayload, *_pIndexList);
    iter.iterate();
  }
    
  return XtcIterL1Accept::Continue;     
}
 
} // namespace Index 
} // namespace Pds
