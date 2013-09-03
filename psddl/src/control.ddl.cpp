
// *** Do not edit this file, it is auto-generated ***

#include "pdsdata/psddl/control.ddl.h"

#include <iostream>
namespace Pds {
namespace ControlData {
uint8_t
PVControl::array() const {
  return _index != NoArray;
}
uint8_t
PVMonitor::array() const {
  return _index != NoArray;
}
ConfigV1::ConfigV1(uint32_t arg__bf_events, uint8_t arg__bf_uses_duration, uint8_t arg__bf_uses_events, const Pds::ClockTime& arg__duration, uint32_t arg__npvControls, uint32_t arg__npvMonitors, const ControlData::PVControl* arg__pvControls, const ControlData::PVMonitor* arg__pvMonitors)
    : _control(((arg__bf_events) & 0x3fffffff)|(((arg__bf_uses_duration) & 0x1)<<30)|(((arg__bf_uses_events) & 0x1)<<31)), _duration(arg__duration), _npvControls(arg__npvControls), _npvMonitors(arg__npvMonitors)
{
  if (arg__pvControls and (this->npvControls())) {
    ptrdiff_t offset = 24;
    ControlData::PVControl* data = reinterpret_cast<ControlData::PVControl*>(((char*)this)+offset);
    std::copy(arg__pvControls, arg__pvControls+(this->npvControls()), data);
  }
  if (arg__pvMonitors and (this->npvMonitors())) {
    ptrdiff_t offset = 24+(44*(this->npvControls()));
    ControlData::PVMonitor* data = reinterpret_cast<ControlData::PVMonitor*>(((char*)this)+offset);
    std::copy(arg__pvMonitors, arg__pvMonitors+(this->npvMonitors()), data);
  }
}
ConfigV2::ConfigV2(uint32_t arg__bf_events, uint8_t arg__bf_uses_duration, uint8_t arg__bf_uses_events, const Pds::ClockTime& arg__duration, uint32_t arg__npvControls, uint32_t arg__npvMonitors, uint32_t arg__npvLabels, const ControlData::PVControl* arg__pvControls, const ControlData::PVMonitor* arg__pvMonitors, const ControlData::PVLabel* arg__pvLabels)
    : _control(((arg__bf_events) & 0x3fffffff)|(((arg__bf_uses_duration) & 0x1)<<30)|(((arg__bf_uses_events) & 0x1)<<31)), _duration(arg__duration), _npvControls(arg__npvControls), _npvMonitors(arg__npvMonitors), _npvLabels(arg__npvLabels)
{
  if (arg__pvControls and (this->npvControls())) {
    ptrdiff_t offset = 28;
    ControlData::PVControl* data = reinterpret_cast<ControlData::PVControl*>(((char*)this)+offset);
    std::copy(arg__pvControls, arg__pvControls+(this->npvControls()), data);
  }
  if (arg__pvMonitors and (this->npvMonitors())) {
    ptrdiff_t offset = 28+(44*(this->npvControls()));
    ControlData::PVMonitor* data = reinterpret_cast<ControlData::PVMonitor*>(((char*)this)+offset);
    std::copy(arg__pvMonitors, arg__pvMonitors+(this->npvMonitors()), data);
  }
  if (arg__pvLabels and (this->npvLabels())) {
    ptrdiff_t offset = (28+(44*(this->npvControls())))+(52*(this->npvMonitors()));
    ControlData::PVLabel* data = reinterpret_cast<ControlData::PVLabel*>(((char*)this)+offset);
    std::copy(arg__pvLabels, arg__pvLabels+(this->npvLabels()), data);
  }
}
} // namespace ControlData
} // namespace Pds