#include "pdsdata/control/PVMonitor.hh"

#include <string.h>

using namespace Pds::ControlData;

PVMonitor::PVMonitor() {}

PVMonitor::PVMonitor(const char* pvname, unsigned restrictedAlarms) :
  _restricted(restrictedAlarms)
{
  strncpy(_name, pvname, NameSize);
}

PVMonitor::PVMonitor(const PVMonitor& m) :
  _restricted(m._restricted)
{
  strncpy(_name, m._name, NameSize);
}

PVMonitor::~PVMonitor() {}

const char* PVMonitor::name            () const 
{ return _name; }

unsigned    PVMonitor::restrictedAlarms() const
{ return _restricted; }
