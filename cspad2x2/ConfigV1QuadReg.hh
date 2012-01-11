#ifndef Cspad2x2_ConfigV1QuadReg_hh
#define Cspad2x2_ConfigV1QuadReg_hh

#include "pdsdata/cspad2x2/Detector.hh"

#include <stdint.h>

#pragma pack(4)

namespace Pds {
  namespace CsPad2x2 {

    class CsPadDigitalPotsCfg
    {
      public:
        CsPadDigitalPotsCfg() {int i = 0; while (i<PotsPerQuad) pots[i++] = 0;}
        uint8_t         value(unsigned i) const {return pots[i];}
      public:
        uint8_t         pots[PotsPerQuad];
    };

    class CsPadReadOnlyCfg
    {
      public:
        CsPadReadOnlyCfg() : shiftTest(0), version(0) {};
        ~CsPadReadOnlyCfg() {};

      public:
        uint32_t        shiftTest;
        uint32_t        version;
    };

    class CsPadGainMapCfg
    {
      public:
        CsPadGainMapCfg() {};
        typedef uint16_t GainMap[ColumnsPerASIC][MaxRowsPerASIC];
        GainMap*       map()       { return &_gainMap; }
        const GainMap* map() const { return &_gainMap; }
      public:
        GainMap    _gainMap;
    };

    class ConfigV1QuadReg
    {
        class CsPadReadOnlyCfg;
        class CsPadDigitalPotsCfg;
        class CsPadGainMapCfg;

      public:
        ConfigV1QuadReg() {};
        ConfigV1QuadReg(
            uint32_t         shiftSelect[],
            uint32_t         edgeSelect[],
            uint32_t         readClkSet,
            uint32_t         readClkHold,
            uint32_t         dataMode,
            uint32_t         prstSel,
            uint32_t         acqDelay,
            uint32_t         intTime,
            uint32_t         digDelay,
            uint32_t         ampIdle,
            uint32_t         injTotal,
            uint32_t         rowColShiftPer) :
            _readClkSet(readClkSet),
            _readClkHold(readClkHold),
            _dataMode(dataMode),
            _prstSel(prstSel),
            _acqDelay(acqDelay),
            _intTime(intTime),
            _digDelay(digDelay),
            _ampIdle(ampIdle),
            _injTotal(injTotal),
            _rowColShiftPer(rowColShiftPer) {
          _shiftSelect[0] = shiftSelect[0];
          _edgeSelect [0] = edgeSelect [0];
        };

        const uint32_t*                            shiftSelect()        const   { return _shiftSelect; }
        const uint32_t*                            edgeSelect()         const   { return _edgeSelect;  }
        uint32_t                                   readClkSet()         const   { return _readClkSet;     }
        uint32_t                                   readClkHold()        const   { return _readClkHold;    }
        uint32_t                                   dataMode()           const   { return _dataMode;       }
        uint32_t                                   prstSel()            const   { return _prstSel;        }
        uint32_t                                   acqDelay()           const   { return _acqDelay;       }
        uint32_t                                   intTime()            const   { return _intTime;        }
        uint32_t                                   digDelay()           const   { return _digDelay;       }
        uint32_t                                   ampIdle()            const   { return _ampIdle;        }
        uint32_t                                   injTotal()           const   { return _injTotal;       }
        uint32_t                                   rowColShiftPer()     const   { return _rowColShiftPer; }
        Pds::CsPad2x2::CsPadReadOnlyCfg&                          ro      ()        { return _readOnly;       }
        const Pds::CsPad2x2::CsPadReadOnlyCfg&                    ro      ()  const { return _readOnly;       }
        Pds::CsPad2x2::CsPadDigitalPotsCfg&                       dp      ()        { return _digitalPots;    }
        const Pds::CsPad2x2::CsPadDigitalPotsCfg&                 dp      ()  const { return _digitalPots;    }
        Pds::CsPad2x2::CsPadGainMapCfg*                           gm      ()        { return &_gainMap;       }
        const Pds::CsPad2x2::CsPadGainMapCfg*                     gm      ()  const { return &_gainMap;       }
        Pds::CsPad2x2::CsPadReadOnlyCfg*                          readOnly()        { return &_readOnly;      }
        const Pds::CsPad2x2::CsPadReadOnlyCfg*                    readOnly()  const { return &_readOnly;      }

      private:
        uint32_t                            _shiftSelect[TwoByTwosPerQuad];
        uint32_t                            _edgeSelect[TwoByTwosPerQuad];
        uint32_t                            _readClkSet;
        uint32_t                            _readClkHold;
        uint32_t                            _dataMode;
        uint32_t                            _prstSel;
        uint32_t                            _acqDelay;
        uint32_t                            _intTime;
        uint32_t                            _digDelay;
        uint32_t                            _ampIdle;
        uint32_t                            _injTotal;
        uint32_t                            _rowColShiftPer;

        Pds::CsPad2x2::CsPadReadOnlyCfg     _readOnly;
        Pds::CsPad2x2::CsPadDigitalPotsCfg  _digitalPots;
        Pds::CsPad2x2::CsPadGainMapCfg      _gainMap;
    };
  };
};

#endif
