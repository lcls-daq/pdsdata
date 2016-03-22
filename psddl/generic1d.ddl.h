#ifndef PDS_GENERIC1D_DDL_H
#define PDS_GENERIC1D_DDL_H 1

// *** Do not edit this file, it is auto-generated ***

#include <vector>
#include <iosfwd>
#include <cstddef>
#include <cstring>
#include "pdsdata/xtc/TypeId.hh"
#include "ndarray/ndarray.h"
namespace Pds {
namespace Generic1D {

/** @class ConfigV0

  
*/

#pragma pack(push,4)

class ConfigV0 {
public:
  enum { TypeId = Pds::TypeId::Id_Generic1DConfig /**< XTC type ID value (from Pds::TypeId class) */ };
  enum { Version = 0 /**< XTC type version number */ };
  enum Sample_Type {
    UINT8,
    UINT16,
    UINT32,
    FLOAT32,
    FLOAT64,
  };
  ConfigV0(uint32_t arg__NChannels, const uint32_t* arg__Length, const uint32_t* arg__SampleType, const int32_t* arg__Offset, const double* arg__Period)
    : _NChannels(arg__NChannels)
  {
    if (arg__Length and (this->_NChannels)) {
      ptrdiff_t offset = 4;
      uint32_t* data = reinterpret_cast<uint32_t*>(((char*)this)+offset);
      std::copy(arg__Length, arg__Length+(this->_NChannels), data);
    }
    if (arg__SampleType and (this->_NChannels)) {
      ptrdiff_t offset = 4+(4*(this->_NChannels));
      uint32_t* data = reinterpret_cast<uint32_t*>(((char*)this)+offset);
      std::copy(arg__SampleType, arg__SampleType+(this->_NChannels), data);
    }
    if (arg__Offset and (this->_NChannels)) {
      ptrdiff_t offset = (4+(4*(this->_NChannels)))+(4*(this->_NChannels));
      int32_t* data = reinterpret_cast<int32_t*>(((char*)this)+offset);
      std::copy(arg__Offset, arg__Offset+(this->_NChannels), data);
    }
    if (arg__Period and (this->_NChannels)) {
      ptrdiff_t offset = ((4+(4*(this->_NChannels)))+(4*(this->_NChannels)))+(4*(this->_NChannels));
      double* data = reinterpret_cast<double*>(((char*)this)+offset);
      std::copy(arg__Period, arg__Period+(this->_NChannels), data);
    }
  }
  ConfigV0() {}
  ConfigV0(const ConfigV0& other) {
    const char* src = reinterpret_cast<const char*>(&other);
    std::copy(src, src+other._sizeof(), reinterpret_cast<char*>(this));
  }
  ConfigV0& operator=(const ConfigV0& other) {
    const char* src = reinterpret_cast<const char*>(&other);
    std::copy(src, src+other._sizeof(), reinterpret_cast<char*>(this));
    return *this;
  }
  /** Number of channels */
  uint32_t NChannels() const { return _NChannels; }
  /** Waveform Length

    Note: this overloaded method accepts shared pointer argument which must point to an object containing
    this instance, the returned ndarray object can be used even after this instance disappears. */
  template <typename T>
  ndarray<const uint32_t, 1> Length(const boost::shared_ptr<T>& owner) const { 
    ptrdiff_t offset=4;
    const uint32_t* data = (const uint32_t*)(((char*)this)+offset);
    return make_ndarray(boost::shared_ptr<const uint32_t>(owner, data), this->_NChannels);
  }
  /** Waveform Length

    Note: this method returns ndarray instance which does not control lifetime
    of the data, do not use returned ndarray after this instance disappears. */
  ndarray<const uint32_t, 1> Length() const { ptrdiff_t offset=4;
  const uint32_t* data = (const uint32_t*)(((char*)this)+offset);
  return make_ndarray(data, this->_NChannels); }
  /** Waveform Sample Type

    Note: this overloaded method accepts shared pointer argument which must point to an object containing
    this instance, the returned ndarray object can be used even after this instance disappears. */
  template <typename T>
  ndarray<const uint32_t, 1> SampleType(const boost::shared_ptr<T>& owner) const { 
    ptrdiff_t offset=4+(4*(this->_NChannels));
    const uint32_t* data = (const uint32_t*)(((char*)this)+offset);
    return make_ndarray(boost::shared_ptr<const uint32_t>(owner, data), this->_NChannels);
  }
  /** Waveform Sample Type

    Note: this method returns ndarray instance which does not control lifetime
    of the data, do not use returned ndarray after this instance disappears. */
  ndarray<const uint32_t, 1> SampleType() const { ptrdiff_t offset=4+(4*(this->_NChannels));
  const uint32_t* data = (const uint32_t*)(((char*)this)+offset);
  return make_ndarray(data, this->_NChannels); }
  /** Waveform Delay Samples

    Note: this overloaded method accepts shared pointer argument which must point to an object containing
    this instance, the returned ndarray object can be used even after this instance disappears. */
  template <typename T>
  ndarray<const int32_t, 1> Offset(const boost::shared_ptr<T>& owner) const { 
    ptrdiff_t offset=(4+(4*(this->_NChannels)))+(4*(this->_NChannels));
    const int32_t* data = (const int32_t*)(((char*)this)+offset);
    return make_ndarray(boost::shared_ptr<const int32_t>(owner, data), this->_NChannels);
  }
  /** Waveform Delay Samples

    Note: this method returns ndarray instance which does not control lifetime
    of the data, do not use returned ndarray after this instance disappears. */
  ndarray<const int32_t, 1> Offset() const { ptrdiff_t offset=(4+(4*(this->_NChannels)))+(4*(this->_NChannels));
  const int32_t* data = (const int32_t*)(((char*)this)+offset);
  return make_ndarray(data, this->_NChannels); }
  /** Waveform Sampling Period

    Note: this overloaded method accepts shared pointer argument which must point to an object containing
    this instance, the returned ndarray object can be used even after this instance disappears. */
  template <typename T>
  ndarray<const double, 1> Period(const boost::shared_ptr<T>& owner) const { 
    ptrdiff_t offset=((4+(4*(this->_NChannels)))+(4*(this->_NChannels)))+(4*(this->_NChannels));
    const double* data = (const double*)(((char*)this)+offset);
    return make_ndarray(boost::shared_ptr<const double>(owner, data), this->_NChannels);
  }
  /** Waveform Sampling Period

    Note: this method returns ndarray instance which does not control lifetime
    of the data, do not use returned ndarray after this instance disappears. */
  ndarray<const double, 1> Period() const { ptrdiff_t offset=((4+(4*(this->_NChannels)))+(4*(this->_NChannels)))+(4*(this->_NChannels));
  const double* data = (const double*)(((char*)this)+offset);
  return make_ndarray(data, this->_NChannels); }
  int32_t data_offset(uint32_t channel) const;
  uint32_t Depth(uint32_t channel) const;
  uint32_t _sizeof() const { return (((((((4+(4*(this->_NChannels)))+(4*(this->_NChannels)))+(4*(this->_NChannels)))+(8*(this->_NChannels)))+4)-1)/4)*4; }
private:
  uint32_t	_NChannels;	/**< Number of channels */
  //uint32_t	_Length[this->_NChannels];
  //uint32_t	_SampleType[this->_NChannels];
  //int32_t	_Offset[this->_NChannels];
  //double	_Period[this->_NChannels];
};
std::ostream& operator<<(std::ostream& str, Generic1D::ConfigV0::Sample_Type enval);
#pragma pack(pop)

/** @class DataV0

  
*/

class ConfigV0;
#pragma pack(push,4)

class DataV0 {
public:
  enum { TypeId = Pds::TypeId::Id_Generic1DData /**< XTC type ID value (from Pds::TypeId class) */ };
  enum { Version = 0 /**< XTC type version number */ };
  DataV0() {}
  uint32_t size_of() const { return _size_of; }
  /**     Note: this overloaded method accepts shared pointer argument which must point to an object containing
    this instance, the returned ndarray object can be used even after this instance disappears. */
  template <typename T>
  ndarray<const uint8_t, 1> _int_data(const boost::shared_ptr<T>& owner) const { 
    ptrdiff_t offset=4;
    const uint8_t* data = (const uint8_t*)(((char*)this)+offset);
    return make_ndarray(boost::shared_ptr<const uint8_t>(owner, data), this->_size_of);
  }
  /**     Note: this method returns ndarray instance which does not control lifetime
    of the data, do not use returned ndarray after this instance disappears. */
  ndarray<const uint8_t, 1> _int_data() const { ptrdiff_t offset=4;
  const uint8_t* data = (const uint8_t*)(((char*)this)+offset);
  return make_ndarray(data, this->_size_of); }
  ndarray<const uint8_t, 1> data_u8(const Generic1D::ConfigV0& cfg, uint32_t channel) const;
  ndarray<const uint16_t, 1> data_u16(const Generic1D::ConfigV0& cfg, uint32_t channel) const;
  ndarray<const uint32_t, 1> data_u32(const Generic1D::ConfigV0& cfg, uint32_t channel) const;
  ndarray<const float, 1> data_f32(const Generic1D::ConfigV0& cfg, uint32_t channel) const;
  ndarray<const double, 1> data_f64(const Generic1D::ConfigV0& cfg, uint32_t channel) const;
  uint32_t _sizeof() const { return ((((4+(1*(this->_size_of)))+4)-1)/4)*4; }
private:
  uint32_t	_size_of;
  //uint8_t	_data[this->_size_of];
};
#pragma pack(pop)
} // namespace Generic1D
} // namespace Pds
#endif // PDS_GENERIC1D_DDL_H
