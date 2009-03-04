
#include "pdsdata/acqiris/AcqDataDescV1.hh"

using namespace Pds;
using namespace Acqiris;

static const unsigned Version=1;

Pds::TypeId AcqDataDescV1::typeId() {
  return Pds::TypeId(Pds::TypeId::Id_AcqConfig,Version);
}

uint64_t AcqTimestampV1::value() const {
  uint64_t ts = _timeStampHi;
  ts = (ts<<32) + (unsigned long)(_timeStampLo);
  return ts;
}

uint64_t AcqTimestampV1::operator-(const AcqTimestampV1& ts) const {
  return value()-ts.value();
}

uint32_t AcqDataDescV1::nbrSamplesInSeg() const {
  return _returnedSamplesPerSeg;
}

uint32_t AcqDataDescV1::nbrSegments() const {
  return _returnedSegments;
}

AcqTimestampV1& AcqDataDescV1::timestamp(uint32_t seg) {
  return _timestamp()[seg];
}

WaveformV1* AcqDataDescV1::waveform(const HorizConfigV1& hconfig) {
  return (WaveformV1*)(&(_timestamp()[hconfig.nbrSegments()]));
}

uint32_t AcqDataDescV1::timestampSize(const HorizConfigV1& hconfig) {
  return hconfig.nbrSegments()*sizeof(AcqTimestampV1);}

uint32_t AcqDataDescV1::waveformSize(const HorizConfigV1& hconfig) {
  return hconfig.nbrSamples()*hconfig.nbrSegments()*sizeof(short)+_extra;}

uint32_t AcqDataDescV1::totalSize(const HorizConfigV1& hconfig) {
  return sizeof(AcqDataDescV1)+AcqDataDescV1::timestampSize(hconfig)+AcqDataDescV1::waveformSize(hconfig);}

AcqDataDescV1* AcqDataDescV1::nextChannel(const HorizConfigV1& hconfig) {
  return (AcqDataDescV1*)((char*)(waveform(hconfig))+AcqDataDescV1::waveformSize(hconfig));
}

AcqTimestampV1* AcqDataDescV1::_timestamp() {
  return (AcqTimestampV1*)(this+1);
}
