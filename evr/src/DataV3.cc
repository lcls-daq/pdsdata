#include "pdsdata/evr/DataV3.hh"
#include "pdsdata/evr/PulseConfig.hh"
#include "pdsdata/evr/OutputMap.hh"

#include <string.h>

using namespace Pds;
using namespace EvrData;

static const unsigned beamOn = 100;
static const unsigned baseRate = 40;
static const unsigned singleShot = 150;

DataV3::DataV3()
{
}

DataV3::DataV3(BeamCode bc,
       RateCode rc,
       unsigned npulses,
       const PulseConfig * pulses,
       unsigned noutputs,
       const OutputMap * outputs):_opcode(opcode(bc, rc)),
_npulses(0), _noutputs(0)
{
  char *next = reinterpret_cast < char *>(this + 1);
  memcpy(next, pulses, npulses * sizeof(PulseConfig));
  _npulses = npulses;
  next += npulses * sizeof(PulseConfig);
  memcpy(next, outputs, noutputs * sizeof(OutputMap));
  _noutputs = noutputs;
}


DataV3::BeamCode DataV3::beam() const
{
  return (_opcode > beamOn) ? On : Off;
}

DataV3::RateCode DataV3::rate() const
{
  RateCode r;
  if (_opcode < beamOn)
    r = RateCode(_opcode - baseRate);
  else if (_opcode < singleShot)
    r = RateCode(_opcode - beamOn - baseRate);
  else
    r = Single;
  return r;
}

unsigned DataV3::opcode() const
{
  return _opcode;
}

unsigned DataV3::opcode(BeamCode bc, RateCode rc)
{
  unsigned v;
  if (rc == Single)
  {
    v = singleShot;
  }
  else
  {
    v = baseRate + unsigned (rc);
    if (bc == On)
      v += beamOn;
  }
  return v;
}

unsigned DataV3::npulses() const
{
  return _npulses;
}
const PulseConfig & DataV3::pulse(unsigned pulse) const
{
  const PulseConfig *p = reinterpret_cast < const PulseConfig * >(this + 1);
  return p[pulse];
}

unsigned DataV3::noutputs() const
{
  return _noutputs;
}
const OutputMap & DataV3::output_map(unsigned output) const
{
  const OutputMap *m =
    reinterpret_cast < const OutputMap * >(&pulse(_npulses));
  return m[output];
}


unsigned DataV3::size() const
{
  return (sizeof(*this) +
    _npulses * sizeof(PulseConfig) + _noutputs * sizeof(OutputMap));
}
