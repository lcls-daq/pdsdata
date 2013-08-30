
// *** Do not edit this file, it is auto-generated ***

#include "pdsdata/psddl/opal1k.ddl.h"

#include <iostream>
namespace Pds {
namespace Opal1k {
ConfigV1::ConfigV1(uint16_t arg__bf_offset, uint16_t arg__bf_gain, Opal1k::ConfigV1::Depth arg__bf_resol, Opal1k::ConfigV1::Binning arg__bf_vbin, Opal1k::ConfigV1::Mirroring arg__bf_mirr, uint8_t arg__bf_vremap, uint8_t arg__bf_corr, uint8_t arg__bf_lookup, uint32_t arg__defectPixelCount, const uint16_t* arg__lookup_table, const Camera::FrameCoord* arg__defectPixels)
    : _offsetAndGain(((arg__bf_offset) & 0xffff)|(((arg__bf_gain) & 0xffff)<<16)), _outputOptions(((arg__bf_resol) & 0xf)|(((arg__bf_vbin) & 0xf)<<4)|(((arg__bf_mirr) & 0xf)<<8)|(((arg__bf_vremap) & 0x1)<<12)|(((arg__bf_corr) & 0x1)<<13)|(((arg__bf_lookup) & 0x1)<<14)), _defectPixelCount(arg__defectPixelCount)
{
  if (arg__lookup_table and (Output_LUT_Size*this->output_lookup_table_enabled())) {
    ptrdiff_t offset = 12;
    uint16_t* data = reinterpret_cast<uint16_t*>(((char*)this)+offset);
    std::copy(arg__lookup_table, arg__lookup_table+(Output_LUT_Size*this->output_lookup_table_enabled()), data);
  }
  if (arg__defectPixels and (this->number_of_defect_pixels())) {
    ptrdiff_t offset = 12+(2*(Output_LUT_Size*this->output_lookup_table_enabled()));
    Camera::FrameCoord* data = reinterpret_cast<Camera::FrameCoord*>(((char*)this)+offset);
    std::copy(arg__defectPixels, arg__defectPixels+(this->number_of_defect_pixels()), data);
  }
}
uint16_t
ConfigV1::output_offset() const {
  return (uint32_t(this->black_level())*uint32_t(this->gain_percent()))/100;
}
uint32_t
ConfigV1::output_resolution_bits() const {
  return this->output_resolution()*2+8;
}
std::ostream& operator<<(std::ostream& str, Opal1k::ConfigV1::Depth enval) {
  const char* val;
  switch (enval) {
  case Opal1k::ConfigV1::Eight_bit:
    val = "Eight_bit";
    break;
  case Opal1k::ConfigV1::Ten_bit:
    val = "Ten_bit";
    break;
  case Opal1k::ConfigV1::Twelve_bit:
    val = "Twelve_bit";
    break;
  default:
    return str << "Depth(" << int(enval) << ")";
  }
  return str << val;
}
std::ostream& operator<<(std::ostream& str, Opal1k::ConfigV1::Binning enval) {
  const char* val;
  switch (enval) {
  case Opal1k::ConfigV1::x1:
    val = "x1";
    break;
  case Opal1k::ConfigV1::x2:
    val = "x2";
    break;
  case Opal1k::ConfigV1::x4:
    val = "x4";
    break;
  case Opal1k::ConfigV1::x8:
    val = "x8";
    break;
  default:
    return str << "Binning(" << int(enval) << ")";
  }
  return str << val;
}
std::ostream& operator<<(std::ostream& str, Opal1k::ConfigV1::Mirroring enval) {
  const char* val;
  switch (enval) {
  case Opal1k::ConfigV1::None:
    val = "None";
    break;
  case Opal1k::ConfigV1::HFlip:
    val = "HFlip";
    break;
  case Opal1k::ConfigV1::VFlip:
    val = "VFlip";
    break;
  case Opal1k::ConfigV1::HVFlip:
    val = "HVFlip";
    break;
  default:
    return str << "Mirroring(" << int(enval) << ")";
  }
  return str << val;
}
} // namespace Opal1k
} // namespace Pds
