#ifndef IMP_CONFIG_V1_HH
#define IMP_CONFIG_V1_HH

#include <stdint.h>

#pragma pack(4)


namespace Pds
{
  namespace Imp
  {

    class ConfigV1
    {
      public:
        static const int Version               = 1;
        ConfigV1();
        ConfigV1(
            unsigned,
            unsigned,
            unsigned,
            unsigned,
            unsigned,
            unsigned,
            unsigned,
            unsigned,
            unsigned,
            unsigned
        );
        ~ConfigV1() {};

        enum Registers {
          Range,
          Cal_range,
          Reset,
          Bias_data,
          Cal_data,
          BiasDac_data,
          Cal_strobe,
          NumberOfSamples,
          TrigDelay,
          Adc_delay,
          NumberOfRegisters
        };

        enum {
          NumberOfValues=NumberOfRegisters,
          MaxNumberOfSamples=0x3ff
        };


        static const int      version() { return Version; }

        uint32_t              get      (Registers);
        const uint32_t        get      (Registers) const;
        uint32_t              set      (Registers, uint32_t);
        static char*          name     (Registers, bool init=false);
        static uint32_t       rangeHigh(Registers);
        static uint32_t       rangeLow (Registers);
        static uint32_t       defaultValue(Registers);


      private:
        uint32_t          _values[NumberOfValues];
    };


  } // namespace Imp

} // namespace Pds 

#pragma pack()

#endif  // IMP_CONFIG_V1_HH
