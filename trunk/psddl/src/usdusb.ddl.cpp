
// *** Do not edit this file, it is auto-generated ***

#include "pdsdata/psddl/usdusb.ddl.h"

#include <iostream>
namespace Pds {
namespace UsdUsb {
std::ostream& operator<<(std::ostream& str, UsdUsb::ConfigV1::Count_Mode enval) {
  const char* val;
  switch (enval) {
  case UsdUsb::ConfigV1::WRAP_FULL:
    val = "WRAP_FULL";
    break;
  case UsdUsb::ConfigV1::LIMIT:
    val = "LIMIT";
    break;
  case UsdUsb::ConfigV1::HALT:
    val = "HALT";
    break;
  case UsdUsb::ConfigV1::WRAP_PRESET:
    val = "WRAP_PRESET";
    break;
  default:
    return str << "Count_Mode(" << int(enval) << ")";
  }
  return str << val;
}
std::ostream& operator<<(std::ostream& str, UsdUsb::ConfigV1::Quad_Mode enval) {
  const char* val;
  switch (enval) {
  case UsdUsb::ConfigV1::CLOCK_DIR:
    val = "CLOCK_DIR";
    break;
  case UsdUsb::ConfigV1::X1:
    val = "X1";
    break;
  case UsdUsb::ConfigV1::X2:
    val = "X2";
    break;
  case UsdUsb::ConfigV1::X4:
    val = "X4";
    break;
  default:
    return str << "Quad_Mode(" << int(enval) << ")";
  }
  return str << val;
}
ndarray<const int32_t, 1>
DataV1::encoder_count() const {
  unsigned shape[1]={Encoder_Inputs};                 ndarray<int32_t,1> res(shape);                 for (unsigned i=0; i!=Encoder_Inputs; ++i) res[i]=int(this->_count[i] << 8)/256;                 return res;
}
} // namespace UsdUsb
} // namespace Pds
