#include "pdsdata/control/PVControl.hh"

using namespace Pds::ControlData;

#include <string.h>

PVControl::PVControl() {}

PVControl::PVControl(const char* pvname, double setValue) :
  _value(setValue)
{
  strncpy(_name, pvname, NameSize);
}

PVControl::PVControl(const PVControl& c) :
  _value(c._value)
{
  strncpy(_name, c._name, NameSize);
}

PVControl::~PVControl() {}

const char* PVControl::name() const { return _name; }

double PVControl::value() const { return _value; }
