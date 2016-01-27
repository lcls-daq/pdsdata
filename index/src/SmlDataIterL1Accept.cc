#include "pdsdata/xtc/DetInfo.hh"
#include "pdsdata/xtc/ProcInfo.hh"
#include "pdsdata/xtc/BldInfo.hh"

#include "pdsdata/index/SmlDataIterL1Accept.hh"

#include <stdio.h>

namespace Pds
{
namespace SmlData
{

int SmlDataIterL1Accept::process(Xtc * xtc)
{
  Level::Type     level             = xtc->src.level();
  bool            bStopUpdate       = false;
  
  if (xtc->damage.value() & (1<<Damage::IncompleteContribution)) 
  {
    _iterationOk = false;
    return SmlDataIterL1Accept::Stop;
  }

  if (xtc->extent < sizeof(Xtc) ||
      (xtc->extent&3) ||
      (xtc->contains.id() >= TypeId::NumberOf) ||
      (xtc->damage.value()==0 && xtc->contains.id() == TypeId::Any) ||
      (xtc->src.level() >= Level::NumberOfLevels)) {
    printf( "SmlDataIterL1Accept::process(): *** Skipping corrupt xtc: src %08x.%08x ctns %08x extent %08x\n",
            xtc->src.log(), xtc->src.phy(), xtc->contains.value(), xtc->extent);
    return SmlDataIterL1Accept::Stop;
  }

  if (level == Level::Segment)
  {
    if ( _depth != 0 )
      printf( "SmlDataIterL1Accept::process(): *** Error depth: Expect 0, but get %d, level %s\n",
        _depth, Level::name(level) );

    //_pIndexList->updateSegment( *xtc );
  }
  else if (level == Level::Source)
  {
    // Source level normally appear at level 1 only, except for Fccd (which shows at level 2)
    if ( _depth != 1 && _depth != 2 )
      printf( "SmlDataIterL1Accept::process(): *** Error depth: Expect level 1 or 2, but get %d, level %s\n",
        _depth, Level::name(level) );

    //_pIndexList->updateSource( *xtc, bStopUpdate );
  }
  else if (level == Level::Reporter)
  {
    if ( _depth != 1 ) {
      if ((_lquiet&1)==0) {
        printf( "SmlDataIterL1Accept::process(): *** Error depth: Expect 1, but get %d, level %s\n",
                _depth, Level::name(level) );
        printf("\tSuppressing further identical messages\n");
        _lquiet |= 1;
      }
    }
    //else
      //_pIndexList->updateReporter( *xtc, bStopUpdate );
  }
  else if (level == Level::Event)
  {
    // Event level is either L3T result or camrecord, the IOC recorder (both depth 0)
    if (_depth != 0) {
      printf( "SmlDataIterL1Accept::process(): *** Error depth: Expect 0, but get %d, level %s\n",
              _depth, Level::name(level) );
    }
  }
  else if (level == Level::Recorder)
  {
  }
  else
  {
    printf( "SmlDataIterL1Accept::process(): *** Error level %s depth = %d", Level::name(level), _depth );
  }

  int64_t   i64OffsetOrg        = _i64Offset;
  int64_t   i64OffsetPayload    = _i64Offset + sizeof(Xtc);
  //  uint32_t  dgramOffsetOrg      = _dgramOffset;
  uint32_t  dgramOffsetPayload  = _dgramOffset + sizeof(Xtc);
  _i64Offset   += sizeof(Xtc) + xtc->sizeofPayload();
  _dgramOffset += sizeof(Xtc) + xtc->sizeofPayload();

  // depth > 0 : Will stop after current node

  if ( bStopUpdate )
    return SmlDataIterL1Accept::Stop;

  // Remaining case: depth == 0 : Will process more segment nodes

  if (xtc->contains.id() != TypeId::Id_Xtc)
  {
    if (xtc->extent < _uSizeThreshold || xtc->contains.id() == TypeId::Id_EvrData)
    {
      _vecXtcInfo[0].uSize += xtc->sizeofPayload() + sizeof(Xtc);
      XtcInfo xtcInfo = {i64OffsetOrg, xtc->extent, int(_depth+1), -1};
      _vecXtcInfo.push_back(xtcInfo);
    }
    else
    {
      _xtcObjPool.resize(_xtcObjPool.size()+1);
      XtcObj& xtcObj = _xtcObjPool.back();
      new ((char*)&xtcObj) Xtc     (*xtc);
      new (xtcObj.proxyV1) ProxyV1 (i64OffsetOrg, xtc->contains, xtc->extent);
      xtcObj.xtc.contains = TypeId(TypeId::Id_SmlDataProxy, 1);
      xtcObj.xtc.extent   = sizeof(Xtc) + sizeof(ProxyV1);
      _vecXtcInfo[0].uSize += xtcObj.xtc.extent;
      XtcInfo xtcInfo = {i64OffsetOrg, xtcObj.xtc.extent, int(_depth+1), int(_xtcObjPool.size()-1)};
      //      printf("  storing data for offset 0x%Lx pool %d extent 0x%x (0x%x)\n", (long long) xtcInfo.i64Offset, xtcInfo.iPoolIndex,xtcInfo.uSize, xtcObj.xtc.extent);
      _vecXtcInfo.push_back(xtcInfo);
    }
  }
  else
  {
    SmlDataIterL1Accept iter(xtc, _depth+1, i64OffsetPayload, dgramOffsetPayload, _uSizeThreshold, _xtcObjPool, _lquiet);
    iter.iterate();

    // don't make proxy for xtc object
    //if (iter.xtcInfoList()[0].uSize < _uSizeThreshold)
    _vecXtcInfo[0].uSize += iter.xtcInfoList()[0].uSize;
    _vecXtcInfo.insert(_vecXtcInfo.end(), iter.xtcInfoList().begin(), iter.xtcInfoList().end());
  }

  return SmlDataIterL1Accept::Continue;
}

} // namespace Index
} // namespace Pds
