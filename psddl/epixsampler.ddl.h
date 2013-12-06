#ifndef PDS_EPIXSAMPLER_DDL_H
#define PDS_EPIXSAMPLER_DDL_H 1

// *** Do not edit this file, it is auto-generated ***

#include <vector>
#include <iosfwd>
#include <cstddef>
#include <cstring>
#include "pdsdata/xtc/TypeId.hh"
#include "ndarray/ndarray.h"
namespace Pds {
namespace EpixSampler {

/** @class ConfigV1

  
*/

#pragma pack(push,4)

class ConfigV1 {
public:
  enum { TypeId = Pds::TypeId::Id_EpixSamplerConfig /**< XTC type ID value (from Pds::TypeId class) */ };
  enum { Version = 1 /**< XTC type version number */ };
  ConfigV1()
  {
  }
  ConfigV1(uint32_t arg__version, uint32_t arg__runTrigDelay, uint32_t arg__daqTrigDelay, uint32_t arg__daqSetting, uint32_t arg__adcClkHalfT, uint32_t arg__adcPipelineDelay, uint32_t arg__digitalCardId0, uint32_t arg__digitalCardId1, uint32_t arg__analogCardId0, uint32_t arg__analogCardId1, uint32_t arg__numberOfChannels, uint32_t arg__samplesPerChannel, uint32_t arg__baseClockFrequency, uint8_t arg__testPatternEnable);
  ConfigV1(const ConfigV1& other) {
    const char* src = reinterpret_cast<const char*>(&other);
    std::copy(src, src+other._sizeof(), reinterpret_cast<char*>(this));
  }
  ConfigV1& operator=(const ConfigV1& other) {
    const char* src = reinterpret_cast<const char*>(&other);
    std::copy(src, src+other._sizeof(), reinterpret_cast<char*>(this));
    return *this;
  }
  uint32_t version() const { return _version; }
  uint32_t runTrigDelay() const { return _runTrigDelay; }
  uint32_t daqTrigDelay() const { return _daqTrigDelay; }
  uint32_t daqSetting() const { return _daqSetting; }
  uint32_t adcClkHalfT() const { return _adcClkHalfT; }
  uint32_t adcPipelineDelay() const { return _adcPipelineDelay; }
  uint32_t digitalCardId0() const { return _digitalCardId0; }
  uint32_t digitalCardId1() const { return _digitalCardId1; }
  uint32_t analogCardId0() const { return _analogCardId0; }
  uint32_t analogCardId1() const { return _analogCardId1; }
  uint32_t numberOfChannels() const { return _numberOfChannels; }
  uint32_t samplesPerChannel() const { return _samplesPerChannel; }
  uint32_t baseClockFrequency() const { return _baseClockFrequency; }
  uint8_t testPatternEnable() const { return uint8_t((this->_bitControls>>8) & 0x1); }
  double sampleInterval_sec() const;
  static uint32_t _sizeof() { return 56; }
private:
  uint32_t	_version;
  uint32_t	_runTrigDelay;
  uint32_t	_daqTrigDelay;
  uint32_t	_daqSetting;
  uint32_t	_adcClkHalfT;
  uint32_t	_adcPipelineDelay;
  uint32_t	_digitalCardId0;
  uint32_t	_digitalCardId1;
  uint32_t	_analogCardId0;
  uint32_t	_analogCardId1;
  uint32_t	_numberOfChannels;
  uint32_t	_samplesPerChannel;
  uint32_t	_baseClockFrequency;
  uint32_t	_bitControls;
};
#pragma pack(pop)

/** @class ElementV1

  
*/

class ConfigV1;
#pragma pack(push,2)

class ElementV1 {
public:
  enum { TypeId = Pds::TypeId::Id_EpixSamplerElement /**< XTC type ID value (from Pds::TypeId class) */ };
  enum { Version = 1 /**< XTC type version number */ };
  ElementV1() {}
private:
  ElementV1(const ElementV1&);
  ElementV1& operator=(const ElementV1&);
public:
  uint8_t vc() const { return uint8_t(this->_first & 0x3); }
  uint8_t lane() const { return uint8_t((this->_first>>6) & 0x3); }
  uint16_t acqCount() const { return uint16_t(this->_second & 0xffff); }
  uint32_t frameNumber() const { return _frameNumber; }
  uint32_t ticks() const { return _ticks; }
  uint32_t fiducials() const { return _fiducials; }
  /**     Note: this overloaded method accepts shared pointer argument which must point to an object containing
    this instance, the returned ndarray object can be used even after this instance disappears. */
  template <typename T>
  ndarray<const uint16_t, 2> frame(const EpixSampler::ConfigV1& cfg, const boost::shared_ptr<T>& owner) const { 
    ptrdiff_t offset=32;
    const uint16_t* data = (const uint16_t*)(((char*)this)+offset);
    return make_ndarray(boost::shared_ptr<const uint16_t>(owner, data), cfg.numberOfChannels(), cfg.samplesPerChannel());
  }
  /**     Note: this method returns ndarray instance which does not control lifetime
    of the data, do not use returned ndarray after this instance disappears. */
  ndarray<const uint16_t, 2> frame(const EpixSampler::ConfigV1& cfg) const { ptrdiff_t offset=32;
  const uint16_t* data = (const uint16_t*)(((char*)this)+offset);
  return make_ndarray(data, cfg.numberOfChannels(), cfg.samplesPerChannel()); }
  /**     Note: this overloaded method accepts shared pointer argument which must point to an object containing
    this instance, the returned ndarray object can be used even after this instance disappears. */
  template <typename T>
  ndarray<const uint16_t, 1> temperatures(const EpixSampler::ConfigV1& cfg, const boost::shared_ptr<T>& owner) const { 
    ptrdiff_t offset=32+(2*((cfg.numberOfChannels())*(cfg.samplesPerChannel())));
    const uint16_t* data = (const uint16_t*)(((char*)this)+offset);
    return make_ndarray(boost::shared_ptr<const uint16_t>(owner, data), cfg.numberOfChannels());
  }
  /**     Note: this method returns ndarray instance which does not control lifetime
    of the data, do not use returned ndarray after this instance disappears. */
  ndarray<const uint16_t, 1> temperatures(const EpixSampler::ConfigV1& cfg) const { ptrdiff_t offset=32+(2*((cfg.numberOfChannels())*(cfg.samplesPerChannel())));
  const uint16_t* data = (const uint16_t*)(((char*)this)+offset);
  return make_ndarray(data, cfg.numberOfChannels()); }
  uint32_t lastWord(const EpixSampler::ConfigV1& cfg) const { ptrdiff_t offset=(32+(2*((cfg.numberOfChannels())*(cfg.samplesPerChannel()))))+(2*(cfg.numberOfChannels()));
  return *(const uint32_t*)(((const char*)this)+offset); }
  static uint32_t _sizeof(const EpixSampler::ConfigV1& cfg) { return ((((((32+(2*(cfg.numberOfChannels())*(cfg.samplesPerChannel())))+(2*(cfg.numberOfChannels())))+4)+2)-1)/2)*2; }
private:
  uint32_t	_first;
  uint32_t	_second;
  uint32_t	_frameNumber;
  uint32_t	_ticks;
  uint32_t	_fiducials;
  uint32_t	_z0;
  uint32_t	_z1;
  uint32_t	_z2;
  //uint16_t	_frame[cfg.numberOfChannels()][cfg.samplesPerChannel()];
  //uint16_t	_temperatures[cfg.numberOfChannels()];
  //uint32_t	_lastWord;
};
#pragma pack(pop)
} // namespace EpixSampler
} // namespace Pds
#endif // PDS_EPIXSAMPLER_DDL_H
