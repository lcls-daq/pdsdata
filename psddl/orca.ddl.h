#ifndef PDS_ORCA_DDL_H
#define PDS_ORCA_DDL_H 1

// *** Do not edit this file, it is auto-generated ***

#include <vector>
#include <iosfwd>
#include <cstddef>
#include "pdsdata/xtc/TypeId.hh"
#include "ndarray/ndarray.h"
namespace Pds {
namespace Orca {

/** @class ConfigV1

  
*/

#pragma pack(push,4)

class ConfigV1 {
public:
  enum { TypeId = Pds::TypeId::Id_OrcaConfig /**< XTC type ID value (from Pds::TypeId class) */ };
  enum { Version = 1 /**< XTC type version number */ };
  enum { Row_Pixels = 2048 };
  enum { Column_Pixels = 2048 };
  enum ReadoutMode {
    x1,
    x2,
    x4,
    Subarray,
  };
  enum Cooling {
    Off,
    On,
    Max,
  };
  ConfigV1(Orca::ConfigV1::ReadoutMode arg__bf_readoutMode, Orca::ConfigV1::Cooling arg__bf_cooling, uint8_t arg__bf_defect_pixel_correction_enabled, uint32_t arg__rows)
    : _options(((arg__bf_readoutMode) & 0x3)|(((arg__bf_cooling) & 0x3)<<2)|(((arg__bf_defect_pixel_correction_enabled) & 0x1)<<4)), _rows(arg__rows)
  {
  }
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
  Orca::ConfigV1::ReadoutMode mode() const { return ReadoutMode(this->_options & 0x3); }
  Orca::ConfigV1::Cooling cooling() const { return Cooling((this->_options>>2) & 0x3); }
  uint8_t defect_pixel_correction_enabled() const { return uint8_t((this->_options>>4) & 0x1); }
  uint32_t rows() const { return _rows; }
  static uint32_t _sizeof() { return 8; }
private:
  uint32_t	_options;
  uint32_t	_rows;
};
std::ostream& operator<<(std::ostream& str, Orca::ConfigV1::ReadoutMode enval);
std::ostream& operator<<(std::ostream& str, Orca::ConfigV1::Cooling enval);
#pragma pack(pop)
} // namespace Orca
} // namespace Pds
#endif // PDS_ORCA_DDL_H