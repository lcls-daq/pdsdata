#ifndef PDS_FLI_DDL_H
#define PDS_FLI_DDL_H 1

// *** Do not edit this file, it is auto-generated ***

#include <vector>
#include <iosfwd>
#include <cstddef>
#include <cstring>
#include "pdsdata/xtc/TypeId.hh"
#include "ndarray/ndarray.h"
namespace Pds {
namespace Fli {

/** @class ConfigV1

  
*/

#pragma pack(push,4)

class ConfigV1 {
public:
  enum { TypeId = Pds::TypeId::Id_FliConfig /**< XTC type ID value (from Pds::TypeId class) */ };
  enum { Version = 1 /**< XTC type version number */ };
  ConfigV1()
  {
  }
  ConfigV1(uint32_t arg__uWidth, uint32_t arg__uHeight, uint32_t arg__uOrgX, uint32_t arg__uOrgY, uint32_t arg__uBinX, uint32_t arg__uBinY, float arg__f32ExposureTime, float arg__f32CoolingTemp, uint8_t arg__u8GainIndex, uint8_t arg__u8ReadoutSpeedIndex, uint16_t arg__u16ExposureEventCode, uint32_t arg__u32NumDelayShots)
    : _uWidth(arg__uWidth), _uHeight(arg__uHeight), _uOrgX(arg__uOrgX), _uOrgY(arg__uOrgY), _uBinX(arg__uBinX), _uBinY(arg__uBinY), _f32ExposureTime(arg__f32ExposureTime), _f32CoolingTemp(arg__f32CoolingTemp), _u8GainIndex(arg__u8GainIndex), _u8ReadoutSpeedIndex(arg__u8ReadoutSpeedIndex), _u16ExposureEventCode(arg__u16ExposureEventCode), _u32NumDelayShots(arg__u32NumDelayShots)
  {
  }
  ConfigV1(const ConfigV1& other) {
    const char* src = reinterpret_cast<const char*>(&other);
    std::copy(src, src+other._sizeof(), reinterpret_cast<char*>(this));
  }
  ConfigV1& operator=(const ConfigV1& other) {
    const char* src = reinterpret_cast<const char*>(&other);
    std::copy(src, src+other._sizeof(), reinterpret_cast<char*>(this));
    return *this;
  }
  uint32_t width() const { return _uWidth; }
  uint32_t height() const { return _uHeight; }
  uint32_t orgX() const { return _uOrgX; }
  uint32_t orgY() const { return _uOrgY; }
  uint32_t binX() const { return _uBinX; }
  uint32_t binY() const { return _uBinY; }
  float exposureTime() const { return _f32ExposureTime; }
  float coolingTemp() const { return _f32CoolingTemp; }
  uint8_t gainIndex() const { return _u8GainIndex; }
  uint8_t readoutSpeedIndex() const { return _u8ReadoutSpeedIndex; }
  uint16_t exposureEventCode() const { return _u16ExposureEventCode; }
  uint32_t numDelayShots() const { return _u32NumDelayShots; }
  /** Total size in bytes of the Frame object, including image and frame header. */
  uint32_t frameSize() const;
  /** calculate frame X size in pixels based on the current ROI and binning settings */
  uint32_t numPixelsX() const { return (this->width() + this->binX() - 1) / this->binX(); }
  /** calculate frame Y size in pixels based on the current ROI and binning settings */
  uint32_t numPixelsY() const { return (this->height() + this->binY() - 1) / this->binY(); }
  /** calculate total frame size in pixels based on the current ROI and binning settings */
  uint32_t numPixels() const { return ((this->width() + this->binX() - 1)/ this->binX())*((this->height() + this->binY() - 1)/ this->binY()); }
  static uint32_t _sizeof() { return 40; }
private:
  uint32_t	_uWidth;
  uint32_t	_uHeight;
  uint32_t	_uOrgX;
  uint32_t	_uOrgY;
  uint32_t	_uBinX;
  uint32_t	_uBinY;
  float	_f32ExposureTime;
  float	_f32CoolingTemp;
  uint8_t	_u8GainIndex;
  uint8_t	_u8ReadoutSpeedIndex;
  uint16_t	_u16ExposureEventCode;
  uint32_t	_u32NumDelayShots;
};
#pragma pack(pop)

/** @class FrameV1

  
*/

class ConfigV1;
#pragma pack(push,4)

class FrameV1 {
public:
  enum { TypeId = Pds::TypeId::Id_FliFrame /**< XTC type ID value (from Pds::TypeId class) */ };
  enum { Version = 1 /**< XTC type version number */ };
  FrameV1(uint32_t iShotIdStart, float fReadoutTime, float fTemperature)
    : _iShotIdStart(iShotIdStart), _fReadoutTime(fReadoutTime), _fTemperature(fTemperature)
  {
  }
  FrameV1() {}
private:
  FrameV1(const FrameV1&);
  FrameV1& operator=(const FrameV1&);
public:
  uint32_t shotIdStart() const { return _iShotIdStart; }
  float readoutTime() const { return _fReadoutTime; }
  float temperature() const { return _fTemperature; }
  /**     Note: this overloaded method accepts shared pointer argument which must point to an object containing
    this instance, the returned ndarray object can be used even after this instance disappears. */
  template <typename T>
  ndarray<const uint16_t, 2> data(const Fli::ConfigV1& cfg, const boost::shared_ptr<T>& owner) const { 
    ptrdiff_t offset=12;
    const uint16_t* data = (const uint16_t*)(((char*)this)+offset);
    return make_ndarray(boost::shared_ptr<const uint16_t>(owner, data), cfg.numPixelsY(), cfg.numPixelsX());
  }
  /**     Note: this method returns ndarray instance which does not control lifetime
    of the data, do not use returned ndarray after this instance disappears. */
  ndarray<const uint16_t, 2> data(const Fli::ConfigV1& cfg) const { ptrdiff_t offset=12;
  const uint16_t* data = (const uint16_t*)(((char*)this)+offset);
  return make_ndarray(data, cfg.numPixelsY(), cfg.numPixelsX()); }
  static uint32_t _sizeof(const Fli::ConfigV1& cfg) { return ((((12+(2*(cfg.numPixelsY())*(cfg.numPixelsX())))+4)-1)/4)*4; }
private:
  uint32_t	_iShotIdStart;
  float	_fReadoutTime;
  float	_fTemperature;
  //uint16_t	_data[cfg.numPixelsY()][cfg.numPixelsX()];
};
#pragma pack(pop)
} // namespace Fli
} // namespace Pds
#endif // PDS_FLI_DDL_H
