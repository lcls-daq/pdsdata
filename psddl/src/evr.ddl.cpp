
// *** Do not edit this file, it is auto-generated ***

#include "pdsdata/psddl/evr.ddl.h"

#include <iostream>
namespace Pds {
namespace EvrData {
int16_t
PulseConfig::trigger() const {
  return this->bf_trigger()-1;
}
int16_t
PulseConfig::set() const {
  return this->bf_set()-1;
}
int16_t
PulseConfig::clear() const {
  return this->bf_clear()-1;
}
std::vector<int>
EventCodeV5::desc_shape() const {
  std::vector<int> shape;
  shape.reserve(1);
  shape.push_back(DescSize);
  return shape;
}
std::vector<int>
EventCodeV6::desc_shape() const {
  std::vector<int> shape;
  shape.reserve(1);
  shape.push_back(DescSize);
  return shape;
}
std::vector<int>
SrcEventCode::desc_shape() const {
  std::vector<int> shape;
  shape.reserve(1);
  shape.push_back(DescSize);
  return shape;
}
uint32_t
OutputMap::map() const {
  
    enum { Pulse_Offset=0, DBus_Offset=32, Prescaler_Offset=40 };
    unsigned src_id = source_id();
    switch(source()) {
    case Pulse     : return src_id + Pulse_Offset;
    case DBus      : return src_id + DBus_Offset;
    case Prescaler : return src_id + Prescaler_Offset;
    case Force_High: return 62;
    case Force_Low : return 63;
    }
    return 0;

}
std::ostream& operator<<(std::ostream& str, EvrData::OutputMap::Source enval) {
  const char* val;
  switch (enval) {
  case EvrData::OutputMap::Pulse:
    val = "Pulse";
    break;
  case EvrData::OutputMap::DBus:
    val = "DBus";
    break;
  case EvrData::OutputMap::Prescaler:
    val = "Prescaler";
    break;
  case EvrData::OutputMap::Force_High:
    val = "Force_High";
    break;
  case EvrData::OutputMap::Force_Low:
    val = "Force_Low";
    break;
  default:
    return str << "Source(" << int(enval) << ")";
  }
  return str << val;
}
std::ostream& operator<<(std::ostream& str, EvrData::OutputMap::Conn enval) {
  const char* val;
  switch (enval) {
  case EvrData::OutputMap::FrontPanel:
    val = "FrontPanel";
    break;
  case EvrData::OutputMap::UnivIO:
    val = "UnivIO";
    break;
  default:
    return str << "Conn(" << int(enval) << ")";
  }
  return str << val;
}
uint32_t
OutputMapV2::map() const {
  
    enum { Pulse_Offset=0, DBus_Offset=32, Prescaler_Offset=40 };
    unsigned src_id = source_id();
    switch(source()) {
    case Pulse     : return src_id + Pulse_Offset;
    case DBus      : return src_id + DBus_Offset;
    case Prescaler : return src_id + Prescaler_Offset;
    case Force_High: return 62;
    case Force_Low : return 63;
    }
    return 0;

}
std::ostream& operator<<(std::ostream& str, EvrData::OutputMapV2::Source enval) {
  const char* val;
  switch (enval) {
  case EvrData::OutputMapV2::Pulse:
    val = "Pulse";
    break;
  case EvrData::OutputMapV2::DBus:
    val = "DBus";
    break;
  case EvrData::OutputMapV2::Prescaler:
    val = "Prescaler";
    break;
  case EvrData::OutputMapV2::Force_High:
    val = "Force_High";
    break;
  case EvrData::OutputMapV2::Force_Low:
    val = "Force_Low";
    break;
  default:
    return str << "Source(" << int(enval) << ")";
  }
  return str << val;
}
std::ostream& operator<<(std::ostream& str, EvrData::OutputMapV2::Conn enval) {
  const char* val;
  switch (enval) {
  case EvrData::OutputMapV2::FrontPanel:
    val = "FrontPanel";
    break;
  case EvrData::OutputMapV2::UnivIO:
    val = "UnivIO";
    break;
  default:
    return str << "Conn(" << int(enval) << ")";
  }
  return str << val;
}
ConfigV1::ConfigV1(uint32_t arg__npulses, uint32_t arg__noutputs, const EvrData::PulseConfig* arg__pulses, const EvrData::OutputMap* arg__output_maps)
    : _npulses(arg__npulses), _noutputs(arg__noutputs)
{
  if (arg__pulses and (this->_npulses)) {
    ptrdiff_t offset = 8;
    EvrData::PulseConfig* data = reinterpret_cast<EvrData::PulseConfig*>(((char*)this)+offset);
    std::copy(arg__pulses, arg__pulses+(this->_npulses), data);
  }
  if (arg__output_maps and (this->_noutputs)) {
    ptrdiff_t offset = 8+(24*(this->_npulses));
    EvrData::OutputMap* data = reinterpret_cast<EvrData::OutputMap*>(((char*)this)+offset);
    std::copy(arg__output_maps, arg__output_maps+(this->_noutputs), data);
  }
}
ConfigV2::ConfigV2(uint32_t arg__opcode, uint32_t arg__npulses, uint32_t arg__noutputs, const EvrData::PulseConfig* arg__pulses, const EvrData::OutputMap* arg__output_maps)
    : _opcode(arg__opcode), _npulses(arg__npulses), _noutputs(arg__noutputs)
{
  if (arg__pulses and (this->_npulses)) {
    ptrdiff_t offset = 12;
    EvrData::PulseConfig* data = reinterpret_cast<EvrData::PulseConfig*>(((char*)this)+offset);
    std::copy(arg__pulses, arg__pulses+(this->_npulses), data);
  }
  if (arg__output_maps and (this->_noutputs)) {
    ptrdiff_t offset = 12+(24*(this->_npulses));
    EvrData::OutputMap* data = reinterpret_cast<EvrData::OutputMap*>(((char*)this)+offset);
    std::copy(arg__output_maps, arg__output_maps+(this->_noutputs), data);
  }
}
EvrData::ConfigV2::BeamCode
ConfigV2::beam() const {
  return (this->_opcode > beamOn) ? On : Off;
}
EvrData::ConfigV2::RateCode
ConfigV2::rate() const {
  
    return (this->_opcode < beamOn) ? RateCode(this->_opcode-baseRate) :
	((this->_opcode < singleShot) ? RateCode(this->_opcode-beamOn-baseRate) : Single);

}
std::ostream& operator<<(std::ostream& str, EvrData::ConfigV2::RateCode enval) {
  const char* val;
  switch (enval) {
  case EvrData::ConfigV2::r120Hz:
    val = "r120Hz";
    break;
  case EvrData::ConfigV2::r60Hz:
    val = "r60Hz";
    break;
  case EvrData::ConfigV2::r30Hz:
    val = "r30Hz";
    break;
  case EvrData::ConfigV2::r10Hz:
    val = "r10Hz";
    break;
  case EvrData::ConfigV2::r5Hz:
    val = "r5Hz";
    break;
  case EvrData::ConfigV2::r1Hz:
    val = "r1Hz";
    break;
  case EvrData::ConfigV2::r0_5Hz:
    val = "r0_5Hz";
    break;
  case EvrData::ConfigV2::Single:
    val = "Single";
    break;
  case EvrData::ConfigV2::NumberOfRates:
    val = "NumberOfRates";
    break;
  default:
    return str << "RateCode(" << int(enval) << ")";
  }
  return str << val;
}
std::ostream& operator<<(std::ostream& str, EvrData::ConfigV2::BeamCode enval) {
  const char* val;
  switch (enval) {
  case EvrData::ConfigV2::Off:
    val = "Off";
    break;
  case EvrData::ConfigV2::On:
    val = "On";
    break;
  default:
    return str << "BeamCode(" << int(enval) << ")";
  }
  return str << val;
}
ConfigV3::ConfigV3(uint32_t arg__neventcodes, uint32_t arg__npulses, uint32_t arg__noutputs, const EvrData::EventCodeV3* arg__eventcodes, const EvrData::PulseConfigV3* arg__pulses, const EvrData::OutputMap* arg__output_maps)
    : _neventcodes(arg__neventcodes), _npulses(arg__npulses), _noutputs(arg__noutputs)
{
  if (arg__eventcodes and (this->_neventcodes)) {
    ptrdiff_t offset = 12;
    EvrData::EventCodeV3* data = reinterpret_cast<EvrData::EventCodeV3*>(((char*)this)+offset);
    std::copy(arg__eventcodes, arg__eventcodes+(this->_neventcodes), data);
  }
  if (arg__pulses and (this->_npulses)) {
    ptrdiff_t offset = 12+(16*(this->_neventcodes));
    EvrData::PulseConfigV3* data = reinterpret_cast<EvrData::PulseConfigV3*>(((char*)this)+offset);
    std::copy(arg__pulses, arg__pulses+(this->_npulses), data);
  }
  if (arg__output_maps and (this->_noutputs)) {
    ptrdiff_t offset = (12+(16*(this->_neventcodes)))+(16*(this->_npulses));
    EvrData::OutputMap* data = reinterpret_cast<EvrData::OutputMap*>(((char*)this)+offset);
    std::copy(arg__output_maps, arg__output_maps+(this->_noutputs), data);
  }
}
ConfigV4::ConfigV4(uint32_t arg__neventcodes, uint32_t arg__npulses, uint32_t arg__noutputs, const EvrData::EventCodeV4* arg__eventcodes, const EvrData::PulseConfigV3* arg__pulses, const EvrData::OutputMap* arg__output_maps)
    : _neventcodes(arg__neventcodes), _npulses(arg__npulses), _noutputs(arg__noutputs)
{
  if (arg__eventcodes and (this->_neventcodes)) {
    ptrdiff_t offset = 12;
    EvrData::EventCodeV4* data = reinterpret_cast<EvrData::EventCodeV4*>(((char*)this)+offset);
    std::copy(arg__eventcodes, arg__eventcodes+(this->_neventcodes), data);
  }
  if (arg__pulses and (this->_npulses)) {
    ptrdiff_t offset = 12+(24*(this->_neventcodes));
    EvrData::PulseConfigV3* data = reinterpret_cast<EvrData::PulseConfigV3*>(((char*)this)+offset);
    std::copy(arg__pulses, arg__pulses+(this->_npulses), data);
  }
  if (arg__output_maps and (this->_noutputs)) {
    ptrdiff_t offset = (12+(24*(this->_neventcodes)))+(16*(this->_npulses));
    EvrData::OutputMap* data = reinterpret_cast<EvrData::OutputMap*>(((char*)this)+offset);
    std::copy(arg__output_maps, arg__output_maps+(this->_noutputs), data);
  }
}
SequencerConfigV1::SequencerConfigV1(EvrData::SequencerConfigV1::Source arg__bf_sync_source, EvrData::SequencerConfigV1::Source arg__bf_beam_source, uint32_t arg__length, uint32_t arg__cycles, const EvrData::SequencerEntry* arg__entries)
    : _source(((arg__bf_sync_source) & 0xff)|(((arg__bf_beam_source) & 0xff)<<8)), _length(arg__length), _cycles(arg__cycles)
{
  if (arg__entries and (this->length())) {
    ptrdiff_t offset = 12;
    EvrData::SequencerEntry* data = reinterpret_cast<EvrData::SequencerEntry*>(((char*)this)+offset);
    std::copy(arg__entries, arg__entries+(this->length()), data);
  }
}
std::ostream& operator<<(std::ostream& str, EvrData::SequencerConfigV1::Source enval) {
  const char* val;
  switch (enval) {
  case EvrData::SequencerConfigV1::r120Hz:
    val = "r120Hz";
    break;
  case EvrData::SequencerConfigV1::r60Hz:
    val = "r60Hz";
    break;
  case EvrData::SequencerConfigV1::r30Hz:
    val = "r30Hz";
    break;
  case EvrData::SequencerConfigV1::r10Hz:
    val = "r10Hz";
    break;
  case EvrData::SequencerConfigV1::r5Hz:
    val = "r5Hz";
    break;
  case EvrData::SequencerConfigV1::r1Hz:
    val = "r1Hz";
    break;
  case EvrData::SequencerConfigV1::r0_5Hz:
    val = "r0_5Hz";
    break;
  case EvrData::SequencerConfigV1::Disable:
    val = "Disable";
    break;
  default:
    return str << "Source(" << int(enval) << ")";
  }
  return str << val;
}
ConfigV5::ConfigV5(uint32_t arg__neventcodes, uint32_t arg__npulses, uint32_t arg__noutputs, const EvrData::EventCodeV5* arg__eventcodes, const EvrData::PulseConfigV3* arg__pulses, const EvrData::OutputMap* arg__output_maps, const EvrData::SequencerConfigV1& arg__seq_config)
    : _neventcodes(arg__neventcodes), _npulses(arg__npulses), _noutputs(arg__noutputs)
{
  if (arg__eventcodes and (this->_neventcodes)) {
    ptrdiff_t offset = 12;
    EvrData::EventCodeV5* data = reinterpret_cast<EvrData::EventCodeV5*>(((char*)this)+offset);
    std::copy(arg__eventcodes, arg__eventcodes+(this->_neventcodes), data);
  }
  if (arg__pulses and (this->_npulses)) {
    ptrdiff_t offset = 12+(40*(this->_neventcodes));
    EvrData::PulseConfigV3* data = reinterpret_cast<EvrData::PulseConfigV3*>(((char*)this)+offset);
    std::copy(arg__pulses, arg__pulses+(this->_npulses), data);
  }
  if (arg__output_maps and (this->_noutputs)) {
    ptrdiff_t offset = (12+(40*(this->_neventcodes)))+(16*(this->_npulses));
    EvrData::OutputMap* data = reinterpret_cast<EvrData::OutputMap*>(((char*)this)+offset);
    std::copy(arg__output_maps, arg__output_maps+(this->_noutputs), data);
  }
  {
    ptrdiff_t offset = ((12+(40*(this->_neventcodes)))+(16*(this->_npulses)))+(4*(this->_noutputs));
    EvrData::OutputMap* data = reinterpret_cast<EvrData::OutputMap*>(((char*)this)+offset);
    new (data) SequencerConfigV1(arg__seq_config);
  }
}
ConfigV6::ConfigV6(uint32_t arg__neventcodes, uint32_t arg__npulses, uint32_t arg__noutputs, const EvrData::EventCodeV5* arg__eventcodes, const EvrData::PulseConfigV3* arg__pulses, const EvrData::OutputMapV2* arg__output_maps, const EvrData::SequencerConfigV1& arg__seq_config)
    : _neventcodes(arg__neventcodes), _npulses(arg__npulses), _noutputs(arg__noutputs)
{
  if (arg__eventcodes and (this->_neventcodes)) {
    ptrdiff_t offset = 12;
    EvrData::EventCodeV5* data = reinterpret_cast<EvrData::EventCodeV5*>(((char*)this)+offset);
    std::copy(arg__eventcodes, arg__eventcodes+(this->_neventcodes), data);
  }
  if (arg__pulses and (this->_npulses)) {
    ptrdiff_t offset = 12+(40*(this->_neventcodes));
    EvrData::PulseConfigV3* data = reinterpret_cast<EvrData::PulseConfigV3*>(((char*)this)+offset);
    std::copy(arg__pulses, arg__pulses+(this->_npulses), data);
  }
  if (arg__output_maps and (this->_noutputs)) {
    ptrdiff_t offset = (12+(40*(this->_neventcodes)))+(16*(this->_npulses));
    EvrData::OutputMapV2* data = reinterpret_cast<EvrData::OutputMapV2*>(((char*)this)+offset);
    std::copy(arg__output_maps, arg__output_maps+(this->_noutputs), data);
  }
  {
    ptrdiff_t offset = ((12+(40*(this->_neventcodes)))+(16*(this->_npulses)))+(4*(this->_noutputs));
    EvrData::OutputMapV2* data = reinterpret_cast<EvrData::OutputMapV2*>(((char*)this)+offset);
    new (data) SequencerConfigV1(arg__seq_config);
  }
}
ConfigV7::ConfigV7(uint32_t arg__neventcodes, uint32_t arg__npulses, uint32_t arg__noutputs, const EvrData::EventCodeV6* arg__eventcodes, const EvrData::PulseConfigV3* arg__pulses, const EvrData::OutputMapV2* arg__output_maps, const EvrData::SequencerConfigV1& arg__seq_config)
    : _neventcodes(arg__neventcodes), _npulses(arg__npulses), _noutputs(arg__noutputs)
{
  if (arg__eventcodes and (this->_neventcodes)) {
    ptrdiff_t offset = 12;
    EvrData::EventCodeV6* data = reinterpret_cast<EvrData::EventCodeV6*>(((char*)this)+offset);
    std::copy(arg__eventcodes, arg__eventcodes+(this->_neventcodes), data);
  }
  if (arg__pulses and (this->_npulses)) {
    ptrdiff_t offset = 12+(44*(this->_neventcodes));
    EvrData::PulseConfigV3* data = reinterpret_cast<EvrData::PulseConfigV3*>(((char*)this)+offset);
    std::copy(arg__pulses, arg__pulses+(this->_npulses), data);
  }
  if (arg__output_maps and (this->_noutputs)) {
    ptrdiff_t offset = (12+(44*(this->_neventcodes)))+(16*(this->_npulses));
    EvrData::OutputMapV2* data = reinterpret_cast<EvrData::OutputMapV2*>(((char*)this)+offset);
    std::copy(arg__output_maps, arg__output_maps+(this->_noutputs), data);
  }
  {
    ptrdiff_t offset = ((12+(44*(this->_neventcodes)))+(16*(this->_npulses)))+(4*(this->_noutputs));
    EvrData::OutputMapV2* data = reinterpret_cast<EvrData::OutputMapV2*>(((char*)this)+offset);
    new (data) SequencerConfigV1(arg__seq_config);
  }
}
SrcConfigV1::SrcConfigV1(uint32_t arg__neventcodes, uint32_t arg__npulses, uint32_t arg__noutputs, const EvrData::SrcEventCode* arg__eventcodes, const EvrData::PulseConfigV3* arg__pulses, const EvrData::OutputMapV2* arg__output_maps)
    : _neventcodes(arg__neventcodes), _npulses(arg__npulses), _noutputs(arg__noutputs)
{
  if (arg__eventcodes and (this->_neventcodes)) {
    ptrdiff_t offset = 12;
    EvrData::SrcEventCode* data = reinterpret_cast<EvrData::SrcEventCode*>(((char*)this)+offset);
    std::copy(arg__eventcodes, arg__eventcodes+(this->_neventcodes), data);
  }
  if (arg__pulses and (this->_npulses)) {
    ptrdiff_t offset = 12+(36*(this->_neventcodes));
    EvrData::PulseConfigV3* data = reinterpret_cast<EvrData::PulseConfigV3*>(((char*)this)+offset);
    std::copy(arg__pulses, arg__pulses+(this->_npulses), data);
  }
  if (arg__output_maps and (this->_noutputs)) {
    ptrdiff_t offset = (12+(36*(this->_neventcodes)))+(16*(this->_npulses));
    EvrData::OutputMapV2* data = reinterpret_cast<EvrData::OutputMapV2*>(((char*)this)+offset);
    std::copy(arg__output_maps, arg__output_maps+(this->_noutputs), data);
  }
}
uint8_t
DataV4::present(uint8_t opcode) const {
  
    uint32_t size = this->numFifoEvents();
    for (uint32_t ii = 0; ii < size; ii++) {
      if (this->fifoEvents()[ii].eventCode() == opcode) {
        return 1;
      }
    }
    return 0;

}
std::vector<int>
IOChannel::name_shape() const {
  std::vector<int> shape;
  shape.reserve(1);
  shape.push_back(NameLength);
  return shape;
}
EvrData::OutputMap::Conn
IOConfigV1::conn() const {
  return OutputMap::Conn(this->_conn);
}
std::vector<int>
IOChannelV2::name_shape() const {
  std::vector<int> shape;
  shape.reserve(1);
  shape.push_back(NameLength);
  return shape;
}
IOConfigV2::IOConfigV2(uint32_t arg__nchannels, const EvrData::IOChannelV2* arg__channels)
    : _nchannels(arg__nchannels)
{
  if (arg__channels and (this->_nchannels)) {
    ptrdiff_t offset = 4;
    EvrData::IOChannelV2* data = reinterpret_cast<EvrData::IOChannelV2*>(((char*)this)+offset);
    std::copy(arg__channels, arg__channels+(this->_nchannels), data);
  }
}
} // namespace EvrData
} // namespace Pds
