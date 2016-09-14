#ifndef Pds_Index_SmlDataIterL1Accept_hh
#define Pds_Index_SmlDataIterL1Accept_hh

#include <vector>
#include "pdsdata/xtc/XtcIterator.hh"
#include "pdsdata/index/IndexList.hh"
#include "pdsdata/psddl/smldata.ddl.h"

namespace Pds
{
namespace SmlData
{

#pragma pack(push,4)
struct XtcObj
{
  Xtc xtc;
  union
  {
    char configV1           [sizeof(ConfigV1)];
    char origDgramOffsetV1  [sizeof(OrigDgramOffsetV1)];
    char proxyV1            [sizeof(ProxyV1 )];
  };
};
#pragma pack(pop)

class SmlDataIterL1Accept: public XtcIterator
{
public:
  enum
  { Stop, Continue };

  SmlDataIterL1Accept(Xtc * xtc, unsigned depth, int64_t i64Offset, uint32_t dgramOffset,
    uint32_t uSizeThreshold, std::vector<XtcObj>& xtcObjPool) :
    XtcIterator(xtc), _depth(depth), _i64Offset(i64Offset), _dgramOffset(dgramOffset),
    _uSizeThreshold(uSizeThreshold), _xtcObjPool(xtcObjPool),
    _lquiet(_vquiet), _vquiet(0), _iterationOk(true)
  {
    XtcInfo xtcInfo = {_i64Offset - int64_t(sizeof(Xtc)), 
                       sizeof(Xtc), int(_depth), -1};
    _vecXtcInfo.push_back(xtcInfo);
  }

  SmlDataIterL1Accept(Xtc * xtc, unsigned depth, int64_t i64Offset, uint32_t dgramOffset,
    int uSizeThreshold, std::vector<XtcObj>& xtcObjPool, unsigned& lquiet ) :
    XtcIterator(xtc), _depth(depth), _i64Offset(i64Offset), _dgramOffset(dgramOffset),
    _uSizeThreshold(uSizeThreshold), _xtcObjPool(xtcObjPool),
    _lquiet(lquiet), _vquiet(0), _iterationOk(true)
  {
    XtcInfo xtcInfo = {_i64Offset - int64_t(sizeof(Xtc)), 
                       sizeof(Xtc), int(_depth), -1};
    _vecXtcInfo.push_back(xtcInfo);
  }

  bool iterationOk() const { return _iterationOk; };

  int  process(Xtc * xtc);

  struct XtcInfo
  {
    int64_t   i64Offset;
    uint32_t  uSize;
    int       depth;
    int       iPoolIndex;
  };

  std::vector<XtcInfo>& xtcInfoList() { return _vecXtcInfo; }

private:
  unsigned              _depth;
  int64_t               _i64Offset;
  uint32_t              _dgramOffset;
  uint32_t              _uSizeThreshold;
  std::vector<XtcObj>&  _xtcObjPool;
  unsigned&             _lquiet;
  unsigned              _vquiet;
  std::vector<XtcInfo>  _vecXtcInfo;
  bool                  _iterationOk;

};

} // namespace Index
} // namespace Pds

#endif // #ifndef Pds_Index_SmlDataIterL1Accept_hh
