
// *** Do not edit this file, it is auto-generated ***

#include "pdsdata/psddl/timetool.ddl.h"

#include <iostream>
namespace Pds {
namespace TimeTool {
std::ostream& operator<<(std::ostream& str, TimeTool::EventLogic::LogicOp enval) {
  const char* val;
  switch (enval) {
  case TimeTool::EventLogic::L_OR:
    val = "L_OR";
    break;
  case TimeTool::EventLogic::L_AND:
    val = "L_AND";
    break;
  case TimeTool::EventLogic::L_OR_NOT:
    val = "L_OR_NOT";
    break;
  case TimeTool::EventLogic::L_AND_NOT:
    val = "L_AND_NOT";
    break;
  default:
    return str << "LogicOp(" << int(enval) << ")";
  }
  return str << val;
}
ConfigV1::ConfigV1(TimeTool::ConfigV1::Axis arg__project_axis, uint8_t arg__write_image, uint8_t arg__write_projections, uint8_t arg__subtract_sideband, uint16_t arg__number_of_weights, uint8_t arg__calib_poly_dim, uint8_t arg__base_name_length, uint16_t arg__number_of_beam_event_codes, uint16_t arg__number_of_laser_event_codes, uint32_t arg__signal_cut, const Camera::FrameCoord& arg__sig_roi_lo, const Camera::FrameCoord& arg__sig_roi_hi, const Camera::FrameCoord& arg__sb_roi_lo, const Camera::FrameCoord& arg__sb_roi_hi, double arg__sb_convergence, double arg__ref_convergence, const TimeTool::EventLogic* arg__beam_logic, const TimeTool::EventLogic* arg__laser_logic, const double* arg__weights, const double* arg__calib_poly, const uint8_t* arg__base_name)
    : _Control(((arg__project_axis) & 0x1)|(((arg__write_image) & 0x1)<<1)|(((arg__write_projections) & 0x1)<<2)|(((arg__subtract_sideband) & 0x1)<<3)|(((arg__number_of_weights) & 0xffff)<<4)|(((arg__calib_poly_dim) & 0xf)<<20)|(((arg__base_name_length) & 0xff)<<24)), _number_of_beam_event_codes(arg__number_of_beam_event_codes), _number_of_laser_event_codes(arg__number_of_laser_event_codes), _signal_cut(arg__signal_cut), _sig_roi_lo(arg__sig_roi_lo), _sig_roi_hi(arg__sig_roi_hi), _sb_roi_lo(arg__sb_roi_lo), _sb_roi_hi(arg__sb_roi_hi), _sb_convergence(arg__sb_convergence), _ref_convergence(arg__ref_convergence)
{
  if (arg__beam_logic and (this->number_of_beam_event_codes())) {
    ptrdiff_t offset = 44;
    TimeTool::EventLogic* data = reinterpret_cast<TimeTool::EventLogic*>(((char*)this)+offset);
    std::copy(arg__beam_logic, arg__beam_logic+(this->number_of_beam_event_codes()), data);
  }
  if (arg__laser_logic and (this->number_of_laser_event_codes())) {
    ptrdiff_t offset = 44+(4*(this->number_of_beam_event_codes()));
    TimeTool::EventLogic* data = reinterpret_cast<TimeTool::EventLogic*>(((char*)this)+offset);
    std::copy(arg__laser_logic, arg__laser_logic+(this->number_of_laser_event_codes()), data);
  }
  if (arg__weights and (this->number_of_weights())) {
    ptrdiff_t offset = (44+(4*(this->number_of_beam_event_codes())))+(4*(this->number_of_laser_event_codes()));
    double* data = reinterpret_cast<double*>(((char*)this)+offset);
    std::copy(arg__weights, arg__weights+(this->number_of_weights()), data);
  }
  if (arg__calib_poly and (this->calib_poly_dim())) {
    ptrdiff_t offset = ((44+(4*(this->number_of_beam_event_codes())))+(4*(this->number_of_laser_event_codes())))+(8*(this->number_of_weights()));
    double* data = reinterpret_cast<double*>(((char*)this)+offset);
    std::copy(arg__calib_poly, arg__calib_poly+(this->calib_poly_dim()), data);
  }
  if (arg__base_name and (this->base_name_length())) {
    ptrdiff_t offset = (((44+(4*(this->number_of_beam_event_codes())))+(4*(this->number_of_laser_event_codes())))+(8*(this->number_of_weights())))+(8*(this->calib_poly_dim()));
    uint8_t* data = reinterpret_cast<uint8_t*>(((char*)this)+offset);
    std::copy(arg__base_name, arg__base_name+(this->base_name_length()), data);
  }
}
std::ostream& operator<<(std::ostream& str, TimeTool::ConfigV1::Axis enval) {
  const char* val;
  switch (enval) {
  case TimeTool::ConfigV1::X:
    val = "X";
    break;
  case TimeTool::ConfigV1::Y:
    val = "Y";
    break;
  default:
    return str << "Axis(" << int(enval) << ")";
  }
  return str << val;
}
} // namespace TimeTool
} // namespace Pds
