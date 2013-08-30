#ifndef PDS_USDUSB_DDL_H
#define PDS_USDUSB_DDL_H 1

// *** Do not edit this file, it is auto-generated ***

#include <vector>
#include <iosfwd>
#include <cstddef>
#include "pdsdata/xtc/TypeId.hh"
#include "ndarray/ndarray.h"
namespace Pds {
namespace UsdUsb {

/** @class ConfigV1

  
*/


class ConfigV1 {
public:
  enum { TypeId = Pds::TypeId::Id_UsdUsbConfig /**< XTC type ID value (from Pds::TypeId class) */ };
  enum { Version = 1 /**< XTC type version number */ };
  enum { NCHANNELS = 4 };
  enum Count_Mode {
    WRAP_FULL,
    LIMIT,
    HALT,
    WRAP_PRESET,
  };
  enum Quad_Mode {
    CLOCK_DIR,
    X1,
    X2,
    X4,
  };
  ConfigV1()
  {
  }
  ConfigV1(const UsdUsb::ConfigV1::Count_Mode* arg__count_mode, const UsdUsb::ConfigV1::Quad_Mode* arg__quad_mode)
  {
    if (arg__count_mode) std::copy(arg__count_mode, arg__count_mode+(4), &_count_mode[0]);
    if (arg__quad_mode) std::copy(arg__quad_mode, arg__quad_mode+(4), &_quad_mode[0]);
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
  /**     Note: this overloaded method accepts shared pointer argument which must point to an object containing
    this instance, the returned ndarray object can be used even after this instance disappears. */
  template <typename T>
  ndarray<const uint32_t, 1> counting_mode(const boost::shared_ptr<T>& owner) const { 
    const uint32_t* data = &_count_mode[0];
    return make_ndarray(boost::shared_ptr<const uint32_t>(owner, data), NCHANNELS);
  }
  /**     Note: this method returns ndarray instance which does not control lifetime
    of the data, do not use returned ndarray after this instance disappears. */
  ndarray<const uint32_t, 1> counting_mode() const { return make_ndarray(&_count_mode[0], NCHANNELS); }
  /**     Note: this overloaded method accepts shared pointer argument which must point to an object containing
    this instance, the returned ndarray object can be used even after this instance disappears. */
  template <typename T>
  ndarray<const uint32_t, 1> quadrature_mode(const boost::shared_ptr<T>& owner) const { 
    const uint32_t* data = &_quad_mode[0];
    return make_ndarray(boost::shared_ptr<const uint32_t>(owner, data), NCHANNELS);
  }
  /**     Note: this method returns ndarray instance which does not control lifetime
    of the data, do not use returned ndarray after this instance disappears. */
  ndarray<const uint32_t, 1> quadrature_mode() const { return make_ndarray(&_quad_mode[0], NCHANNELS); }
  static uint32_t _sizeof() { return (((((0+(4*(NCHANNELS)))+(4*(NCHANNELS)))+4)-1)/4)*4; }
private:
  uint32_t	_count_mode[NCHANNELS];
  uint32_t	_quad_mode[NCHANNELS];
};
std::ostream& operator<<(std::ostream& str, UsdUsb::ConfigV1::Count_Mode enval);
std::ostream& operator<<(std::ostream& str, UsdUsb::ConfigV1::Quad_Mode enval);

/** @class DataV1

  
*/


class DataV1 {
public:
  enum { TypeId = Pds::TypeId::Id_UsdUsbData /**< XTC type ID value (from Pds::TypeId class) */ };
  enum { Version = 1 /**< XTC type version number */ };
  enum { Encoder_Inputs = 4 };
  enum { Analog_Inputs = 4 };
  enum { Digital_Inputs = 8 };
  DataV1() {}
  DataV1(const DataV1& other) {
    const char* src = reinterpret_cast<const char*>(&other);
    std::copy(src, src+other._sizeof(), reinterpret_cast<char*>(this));
  }
  DataV1& operator=(const DataV1& other) {
    const char* src = reinterpret_cast<const char*>(&other);
    std::copy(src, src+other._sizeof(), reinterpret_cast<char*>(this));
    return *this;
  }
  uint8_t digital_in() const { return _din; }
  uint32_t timestamp() const { return _timestamp; }
  /**     Note: this overloaded method accepts shared pointer argument which must point to an object containing
    this instance, the returned ndarray object can be used even after this instance disappears. */
  template <typename T>
  ndarray<const uint8_t, 1> status(const boost::shared_ptr<T>& owner) const { 
    const uint8_t* data = &_status[0];
    return make_ndarray(boost::shared_ptr<const uint8_t>(owner, data), 4);
  }
  /**     Note: this method returns ndarray instance which does not control lifetime
    of the data, do not use returned ndarray after this instance disappears. */
  ndarray<const uint8_t, 1> status() const { return make_ndarray(&_status[0], 4); }
  /**     Note: this overloaded method accepts shared pointer argument which must point to an object containing
    this instance, the returned ndarray object can be used even after this instance disappears. */
  template <typename T>
  ndarray<const uint16_t, 1> analog_in(const boost::shared_ptr<T>& owner) const { 
    const uint16_t* data = &_ain[0];
    return make_ndarray(boost::shared_ptr<const uint16_t>(owner, data), Analog_Inputs);
  }
  /**     Note: this method returns ndarray instance which does not control lifetime
    of the data, do not use returned ndarray after this instance disappears. */
  ndarray<const uint16_t, 1> analog_in() const { return make_ndarray(&_ain[0], Analog_Inputs); }
  /** Return lower 24 bits of _count array as signed integer values. */
  ndarray<const int32_t, 1> encoder_count() const;
  static uint32_t _sizeof() { return ((((((((((0+(1*(6)))+1)+1)+4)+(4*(Encoder_Inputs)))+(1*(4)))+(2*(Analog_Inputs)))+4)-1)/4)*4; }
private:
  uint8_t	_header[6];
  uint8_t	_din;
  uint8_t	_estop;
  uint32_t	_timestamp;
  uint32_t	_count[Encoder_Inputs];
  uint8_t	_status[4];
  uint16_t	_ain[Analog_Inputs];
};
} // namespace UsdUsb
} // namespace Pds
#endif // PDS_USDUSB_DDL_H
