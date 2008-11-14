#ifndef Pds_TypeId_hh
#define Pds_TypeId_hh

#include <stdint.h>

namespace Pds {

  class TypeId {
  public:
    enum Type {Any, Id_Xtc, Id_Frame, Id_Waveform, Id_AcqConfig, NumberOf};

    TypeId(unsigned v);
    TypeId(const TypeId& v);
    TypeId(Type type, uint32_t version);

    Type     id()      const;
    uint32_t version() const;
    uint32_t value()   const;

    static const char* TypeId::name(Type type);
    
  private:
    uint32_t _value;
  };

}

#endif
