
// *** Do not edit this file, it is auto-generated ***

#include "pdsdata/psddl/archon.ddl.h"

#include <iostream>
namespace Pds {
namespace Archon {
ConfigV1::ConfigV1(Archon::ConfigV1::ReadoutMode arg__readoutMode, uint16_t arg__sweepCount, uint32_t arg__integrationTime, uint32_t arg__nonIntegrationTime, uint32_t arg__preSkipPixels, uint32_t arg__pixels, uint32_t arg__postSkipPixels, uint32_t arg__overscanPixels, uint16_t arg__preSkipLines, uint16_t arg__lines, uint16_t arg__postSkipLines, uint16_t arg__overScanLines, uint16_t arg__horizontalBinning, uint16_t arg__verticalBinning, uint16_t arg__rgh, uint16_t arg__rgl, uint16_t arg__shp, uint16_t arg__shd, uint16_t arg__st, uint16_t arg__stm1, uint16_t arg__at, uint16_t arg__dwell1, uint16_t arg__dwell2, int16_t arg__rgHigh, int16_t arg__rgLow, int16_t arg__sHigh, int16_t arg__sLow, int16_t arg__aHigh, int16_t arg__aLow, int16_t arg__rgSlew, int16_t arg__sSlew, int16_t arg__aSlew, const char* arg__config)
    : _readoutMode(arg__readoutMode), _sweepCount(arg__sweepCount), _integrationTime(arg__integrationTime), _nonIntegrationTime(arg__nonIntegrationTime), _preSkipPixels(arg__preSkipPixels), _pixels(arg__pixels), _postSkipPixels(arg__postSkipPixels), _overscanPixels(arg__overscanPixels), _preSkipLines(arg__preSkipLines), _lines(arg__lines), _postSkipLines(arg__postSkipLines), _overScanLines(arg__overScanLines), _horizontalBinning(arg__horizontalBinning), _verticalBinning(arg__verticalBinning), _rgh(arg__rgh), _rgl(arg__rgl), _shp(arg__shp), _shd(arg__shd), _st(arg__st), _stm1(arg__stm1), _at(arg__at), _dwell1(arg__dwell1), _dwell2(arg__dwell2), _rgHigh(arg__rgHigh), _rgLow(arg__rgLow), _sHigh(arg__sHigh), _sLow(arg__sLow), _aHigh(arg__aHigh), _aLow(arg__aLow), _rgSlew(arg__rgSlew), _sSlew(arg__sSlew), _aSlew(arg__aSlew)
{
  if (arg__config) std::copy(arg__config, arg__config+(256), &_config[0]);
}
std::ostream& operator<<(std::ostream& str, Archon::ConfigV1::ReadoutMode enval) {
  const char* val;
  switch (enval) {
  case Archon::ConfigV1::Single:
    val = "Single";
    break;
  case Archon::ConfigV1::Continuous:
    val = "Continuous";
    break;
  case Archon::ConfigV1::Triggered:
    val = "Triggered";
    break;
  default:
    return str << "ReadoutMode(" << int(enval) << ")";
  }
  return str << val;
}
} // namespace Archon
} // namespace Pds
