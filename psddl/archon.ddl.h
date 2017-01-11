#ifndef PDS_ARCHON_DDL_H
#define PDS_ARCHON_DDL_H 1

// *** Do not edit this file, it is auto-generated ***

#include <vector>
#include <iosfwd>
#include <cstddef>
#include <cstring>
#include "pdsdata/xtc/TypeId.hh"
#include "ndarray/ndarray.h"
namespace Pds {
namespace Archon {

/** @class ConfigV1

  Class containing configuration data for CCDs using the Archon controller.
*/


class ConfigV1 {
public:
  enum { TypeId = Pds::TypeId::Id_ArchonConfig /**< XTC type ID value (from Pds::TypeId class) */ };
  enum { Version = 1 /**< XTC type version number */ };
  enum { FILENAME_CHAR_MAX = 256 };
  enum ReadoutMode {
    Single = 0,
    Continuous = 1,
    Triggered = 2,
  };
  ConfigV1(Archon::ConfigV1::ReadoutMode arg__readoutMode, uint16_t arg__sweepCount, uint32_t arg__integrationTime, uint32_t arg__nonIntegrationTime, uint32_t arg__preSkipPixels, uint32_t arg__pixels, uint32_t arg__postSkipPixels, uint32_t arg__overscanPixels, uint16_t arg__preSkipLines, uint16_t arg__lines, uint16_t arg__postSkipLines, uint16_t arg__overScanLines, uint16_t arg__horizontalBinning, uint16_t arg__verticalBinning, uint16_t arg__rgh, uint16_t arg__rgl, uint16_t arg__shp, uint16_t arg__shd, uint16_t arg__st, uint16_t arg__stm1, uint16_t arg__at, uint16_t arg__dwell1, uint16_t arg__dwell2, int16_t arg__rgHigh, int16_t arg__rgLow, int16_t arg__sHigh, int16_t arg__sLow, int16_t arg__aHigh, int16_t arg__aLow, int16_t arg__rgSlew, int16_t arg__sSlew, int16_t arg__aSlew, const char* arg__config);
  ConfigV1() {}
  ConfigV1(const ConfigV1& other) {
    const char* src = reinterpret_cast<const char*>(&other);
    std::copy(src, src+other._sizeof(), reinterpret_cast<char*>(this));
  }
  ConfigV1& operator=(const ConfigV1& other) {
    const char* src = reinterpret_cast<const char*>(&other);
    std::copy(src, src+other._sizeof(), reinterpret_cast<char*>(this));
    return *this;
  }
  Archon::ConfigV1::ReadoutMode readoutMode() const { return Archon::ConfigV1::ReadoutMode(_readoutMode); }
  uint16_t sweepCount() const { return _sweepCount; }
  uint32_t integrationTime() const { return _integrationTime; }
  uint32_t nonIntegrationTime() const { return _nonIntegrationTime; }
  uint32_t preSkipPixels() const { return _preSkipPixels; }
  uint32_t pixels() const { return _pixels; }
  uint32_t postSkipPixels() const { return _postSkipPixels; }
  uint32_t overscanPixels() const { return _overscanPixels; }
  uint16_t preSkipLines() const { return _preSkipLines; }
  uint16_t lines() const { return _lines; }
  uint16_t postSkipLines() const { return _postSkipLines; }
  uint16_t overScanLines() const { return _overScanLines; }
  uint16_t horizontalBinning() const { return _horizontalBinning; }
  uint16_t verticalBinning() const { return _verticalBinning; }
  uint16_t rgh() const { return _rgh; }
  uint16_t rgl() const { return _rgl; }
  uint16_t shp() const { return _shp; }
  uint16_t shd() const { return _shd; }
  uint16_t st() const { return _st; }
  uint16_t stm1() const { return _stm1; }
  uint16_t at() const { return _at; }
  uint16_t dwell1() const { return _dwell1; }
  uint16_t dwell2() const { return _dwell2; }
  int16_t rgHigh() const { return _rgHigh; }
  int16_t rgLow() const { return _rgLow; }
  int16_t sHigh() const { return _sHigh; }
  int16_t sLow() const { return _sLow; }
  int16_t aHigh() const { return _aHigh; }
  int16_t aLow() const { return _aLow; }
  int16_t rgSlew() const { return _rgSlew; }
  int16_t sSlew() const { return _sSlew; }
  int16_t aSlew() const { return _aSlew; }
  /** The path to an acf file to use with the camera. */
  const char* config() const { return _config; }
  static uint32_t _sizeof() { return ((((76+(1*(FILENAME_CHAR_MAX)))+4)-1)/4)*4; }
private:
  uint16_t	_readoutMode;
  uint16_t	_sweepCount;
  uint32_t	_integrationTime;
  uint32_t	_nonIntegrationTime;
  uint32_t	_preSkipPixels;
  uint32_t	_pixels;
  uint32_t	_postSkipPixels;
  uint32_t	_overscanPixels;
  uint16_t	_preSkipLines;
  uint16_t	_lines;
  uint16_t	_postSkipLines;
  uint16_t	_overScanLines;
  uint16_t	_horizontalBinning;
  uint16_t	_verticalBinning;
  uint16_t	_rgh;
  uint16_t	_rgl;
  uint16_t	_shp;
  uint16_t	_shd;
  uint16_t	_st;
  uint16_t	_stm1;
  uint16_t	_at;
  uint16_t	_dwell1;
  uint16_t	_dwell2;
  int16_t	_rgHigh;
  int16_t	_rgLow;
  int16_t	_sHigh;
  int16_t	_sLow;
  int16_t	_aHigh;
  int16_t	_aLow;
  int16_t	_rgSlew;
  int16_t	_sSlew;
  int16_t	_aSlew;
  char	_config[FILENAME_CHAR_MAX];	/**< The path to an acf file to use with the camera. */
};
std::ostream& operator<<(std::ostream& str, Archon::ConfigV1::ReadoutMode enval);
} // namespace Archon
} // namespace Pds
#endif // PDS_ARCHON_DDL_H