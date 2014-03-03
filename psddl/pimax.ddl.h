#ifndef PDS_PIMAX_DDL_H
#define PDS_PIMAX_DDL_H 1

// *** Do not edit this file, it is auto-generated ***

#include <vector>
#include <iosfwd>
#include <cstddef>
#include <cstring>
#include "pdsdata/xtc/TypeId.hh"
#include "ndarray/ndarray.h"
namespace Pds {
namespace Pimax {

/** @class ConfigV1

  
*/

#pragma pack(push,4)

class ConfigV1 {
public:
  enum { TypeId = Pds::TypeId::Id_PimaxConfig /**< XTC type ID value (from Pds::TypeId class) */ };
  enum { Version = 1 /**< XTC type version number */ };
  ConfigV1(uint32_t arg__uWidth, uint32_t arg__uHeight, uint32_t arg__uOrgX, uint32_t arg__uOrgY, uint32_t arg__uBinX, uint32_t arg__uBinY, float arg__f32ExposureTime, float arg__f32CoolingTemp, float arg__f32ReadoutSpeed, uint16_t arg__u16GainIndex, uint16_t arg__u16IntesifierGain, double arg__f64GateDelay, double arg__f64GateWidth, uint32_t arg__u32MaskedHeight, uint32_t arg__u32KineticHeight, float arg__f32VsSpeed, int16_t arg__i16InfoReportInterval, uint16_t arg__u16ExposureEventCode, uint32_t arg__u32NumIntegrationShots)
    : _uWidth(arg__uWidth), _uHeight(arg__uHeight), _uOrgX(arg__uOrgX), _uOrgY(arg__uOrgY), _uBinX(arg__uBinX), _uBinY(arg__uBinY), _f32ExposureTime(arg__f32ExposureTime), _f32CoolingTemp(arg__f32CoolingTemp), _f32ReadoutSpeed(arg__f32ReadoutSpeed), _u16GainIndex(arg__u16GainIndex), _u16IntesifierGain(arg__u16IntesifierGain), _f64GateDelay(arg__f64GateDelay), _f64GateWidth(arg__f64GateWidth), _u32MaskedHeight(arg__u32MaskedHeight), _u32KineticHeight(arg__u32KineticHeight), _f32VsSpeed(arg__f32VsSpeed), _i16InfoReportInterval(arg__i16InfoReportInterval), _u16ExposureEventCode(arg__u16ExposureEventCode), _u32NumIntegrationShots(arg__u32NumIntegrationShots)
  {
  }
  ConfigV1(uint32_t width, uint32_t height)
    : _uWidth(width), _uHeight(height)
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
  uint32_t width() const { return _uWidth; }
  uint32_t height() const { return _uHeight; }
  uint32_t orgX() const { return _uOrgX; }
  uint32_t orgY() const { return _uOrgY; }
  uint32_t binX() const { return _uBinX; }
  uint32_t binY() const { return _uBinY; }
  float exposureTime() const { return _f32ExposureTime; }
  float coolingTemp() const { return _f32CoolingTemp; }
  float readoutSpeed() const { return _f32ReadoutSpeed; }
  uint16_t gainIndex() const { return _u16GainIndex; }
  uint16_t intesifierGain() const { return _u16IntesifierGain; }
  double gateDelay() const { return _f64GateDelay; }
  double gateWidth() const { return _f64GateWidth; }
  uint32_t maskedHeight() const { return _u32MaskedHeight; }
  uint32_t kineticHeight() const { return _u32KineticHeight; }
  float vsSpeed() const { return _f32VsSpeed; }
  int16_t infoReportInterval() const { return _i16InfoReportInterval; }
  uint16_t exposureEventCode() const { return _u16ExposureEventCode; }
  uint32_t numIntegrationShots() const { return _u32NumIntegrationShots; }
  /** Total size in bytes of the Frame object */
  uint32_t frameSize() const;
  /** calculate frame X size in pixels based on the current ROI and binning settings */
  uint32_t numPixelsX() const { return (this->width() + this->binX() - 1) / this->binX(); }
  /** calculate frame Y size in pixels based on the current ROI and binning settings */
  uint32_t numPixelsY() const { return (this->height()+ this->binY() - 1) / this->binY(); }
  /** calculate total frame size in pixels based on the current ROI and binning settings */
  uint32_t numPixels() const { 
    return ((this->width() + this->binX()-1)/ this->binX() )*((this->height()+ this->binY()-1)/ this->binY() );
 }
  static uint32_t _sizeof() { return 76; }
private:
  uint32_t	_uWidth;
  uint32_t	_uHeight;
  uint32_t	_uOrgX;
  uint32_t	_uOrgY;
  uint32_t	_uBinX;
  uint32_t	_uBinY;
  float	_f32ExposureTime;
  float	_f32CoolingTemp;
  float	_f32ReadoutSpeed;
  uint16_t	_u16GainIndex;
  uint16_t	_u16IntesifierGain;
  double	_f64GateDelay;
  double	_f64GateWidth;
  uint32_t	_u32MaskedHeight;
  uint32_t	_u32KineticHeight;
  float	_f32VsSpeed;
  int16_t	_i16InfoReportInterval;
  uint16_t	_u16ExposureEventCode;
  uint32_t	_u32NumIntegrationShots;
};
#pragma pack(pop)

/** @class FrameV1

  
*/

class ConfigV1;
#pragma pack(push,4)

class FrameV1 {
public:
  enum { TypeId = Pds::TypeId::Id_PimaxFrame /**< XTC type ID value (from Pds::TypeId class) */ };
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
  ndarray<const uint16_t, 2> data(const Pimax::ConfigV1& cfg, const boost::shared_ptr<T>& owner) const { 
    ptrdiff_t offset=12;
    const uint16_t* data = (const uint16_t*)(((char*)this)+offset);
    return make_ndarray(boost::shared_ptr<const uint16_t>(owner, data), cfg.numPixelsY(), cfg.numPixelsX());
  }
  /**     Note: this method returns ndarray instance which does not control lifetime
    of the data, do not use returned ndarray after this instance disappears. */
  ndarray<const uint16_t, 2> data(const Pimax::ConfigV1& cfg) const { ptrdiff_t offset=12;
  const uint16_t* data = (const uint16_t*)(((char*)this)+offset);
  return make_ndarray(data, cfg.numPixelsY(), cfg.numPixelsX()); }
  static uint32_t _sizeof(const Pimax::ConfigV1& cfg) { return ((((12+(2*(cfg.numPixelsY())*(cfg.numPixelsX())))+4)-1)/4)*4; }
private:
  uint32_t	_iShotIdStart;
  float	_fReadoutTime;
  float	_fTemperature;
  //uint16_t	_data[cfg.numPixelsY()][cfg.numPixelsX()];
};
#pragma pack(pop)
} // namespace Pimax
} // namespace Pds
#endif // PDS_PIMAX_DDL_H
