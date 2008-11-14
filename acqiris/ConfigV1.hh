#ifndef ACQIRISCONFIGV1_HH
#define ACQIRISCONFIGV1_HH

#include "pdsdata/xtc/TypeId.hh"
#include <stdint.h>

namespace Pds {

namespace Acqiris {

  class ConfigV1 {
  public:
    ConfigV1 ();
    ConfigV1 (double   sampInterval,
              double   delayTime,
              uint32_t nbrSamples,
              uint32_t nbrSegments,
              uint32_t coupling,
              uint32_t bandwidth,
              double   fullScale,
              double   offset,
              uint32_t trigCoupling,
              uint32_t trigInput,
              uint32_t trigSlope,
              double   trigLevel);
    double   sampInterval() const;
    double   delayTime()    const;
    uint32_t nbrSamples()   const;
    uint32_t nbrSegments()  const;
    uint32_t coupling()     const;
    uint32_t bandwidth()    const;
    double   fullScale()    const;
    double   offset()       const;
    uint32_t trigCoupling() const;
    uint32_t trigInput()    const;
    uint32_t trigSlope()    const;
    double   trigLevel()    const;
    static Pds::TypeId typeId();
  private:
    double   _sampInterval;
    double   _delayTime;
    uint32_t _nbrSamples;
    uint32_t _nbrSegments;
    uint32_t _coupling;
    uint32_t _bandwidth;
    double   _fullScale;
    double   _offset;
    uint32_t _trigCoupling;
    uint32_t _trigInput;
    uint32_t _trigSlope;
    double   _trigLevel;
  };
}
}

#endif
