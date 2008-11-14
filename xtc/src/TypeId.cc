
#include "pdsdata/xtc/TypeId.hh"

using namespace Pds;

TypeId::TypeId(Type type, uint32_t version) :
  _value((version<<16)|type) {}

TypeId::TypeId(unsigned v) : _value(v) {}

TypeId::TypeId(const TypeId& v) : _value(v._value) {}

uint32_t TypeId::value() const {return _value;}

uint32_t TypeId::version() const {return (_value&0xffff0000)>>16;}

TypeId::Type TypeId::id() const {return (TypeId::Type)(_value&0xffff);}

const char* TypeId::name(Type type)
{ 
  static const char* _names[] = {
    "Any",
    "Xtc",
    "Frame",
    "Waveform",
    "AcqConfig",
  };
  return (type < NumberOf ? _names[type] : "-Invalid-");
}
