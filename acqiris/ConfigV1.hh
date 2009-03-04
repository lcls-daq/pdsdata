#ifndef ACQIRISCONFIGV1_HH
#define ACQIRISCONFIGV1_HH

#include "pdsdata/xtc/TypeId.hh"
#include <stdint.h>

namespace Pds {

  namespace Acqiris {

    class VertConfigV1 {
    public:
      VertConfigV1();
      VertConfigV1(double fullScale, double offset,
                   uint32_t coupling, uint32_t bandwidth);
      double   fullScale() const;
      double   offset()    const;
      uint32_t coupling()  const;
      uint32_t bandwidth() const;
    private:
      double   _fullScale;
      double   _offset;
      uint32_t _coupling;
      uint32_t _bandwidth;
    };

    class HorizConfigV1 {
    public:
      HorizConfigV1();
      HorizConfigV1(double   sampInterval,
                    double   delayTime,
                    uint32_t nbrSamples,
                    uint32_t nbrSegments);
      double   sampInterval()     const;
      double   delayTime()        const;
      uint32_t nbrSamples()       const;
      uint32_t nbrSegments()      const;
    private:
      double   _sampInterval;
      double   _delayTime;
      uint32_t _nbrSamples;
      uint32_t _nbrSegments;
    };

    class TrigConfigV1 {
    public:
      TrigConfigV1();
      TrigConfigV1(uint32_t trigCoupling,
                   uint32_t trigInput,
                   uint32_t trigSlope,
                   double   trigLevel);
      uint32_t trigCoupling()     const;
      uint32_t trigInput()        const;
      uint32_t trigSlope()        const;
      double   trigLevel()        const;
    private:
      uint32_t _trigCoupling;
      uint32_t _trigInput;
      uint32_t _trigSlope;
      double   _trigLevel;
    };

    class ConfigV1 {
    public:
      enum {MaxChan=20};
      ConfigV1 ();
      ConfigV1 (uint32_t nbrConvertersPerChannel,
                uint32_t channelMask,
                uint32_t nbrBanks,
                const TrigConfigV1& trigConfig,
                const HorizConfigV1& horizConfig,
                const VertConfigV1* chanConfig);
      uint32_t nbrConvertersPerChannel() const;
      uint32_t channelMask()      const;
      uint32_t nbrChannels()      const;
      uint32_t nbrBanks()         const;
      HorizConfigV1& horizConfig();
      TrigConfigV1& trigConfig();
      VertConfigV1& vertConfig(uint32_t channel);
      const HorizConfigV1& horizConfig() const;
      const TrigConfigV1& trigConfig() const;
      const VertConfigV1& vertConfig(uint32_t channel) const;
      static Pds::TypeId typeId();
    private:
      uint32_t _nbrConvertersPerChannel;
      uint32_t _channelMask;
      uint32_t _nbrBanks;
      TrigConfigV1 _trigConfig;
      HorizConfigV1 _horizConfig;
      VertConfigV1 _vertConfig[MaxChan];
    };

  }
}

#endif
