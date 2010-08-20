#ifndef CSPAD_CONFIG_V1_HH
#define CSPAD_CONFIG_V1_HH

#include <stdint.h>

#pragma pack(4)

namespace Pds
{

  namespace CsPad
  {
    enum {MaxQuadsPerSensor=4, ASICsPerQuad=16};
    enum {RowsPerBank=26, FullBanksPerASIC=7, BanksPerASIC=8, ColumnsPerASIC=185, MaxRowsPerASIC=194};
    enum {PotsPerQuad=80};
    enum RunModes  {NoRunning, RunButDrop, RunAndSendToRCE, RunAndSendTriggeredByTTL, ExternalTriggerSendToRCE, ExternalTriggerDrop, NumberOfRunModes };
    enum DataModes {normal=0, shiftTest=1, testData=2, reserved=3};

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
        ConfigV1QuadReg(
            uint32_t         shiftSelect       = 0,
            uint32_t         edgeSelect        = 0,
            uint32_t         readClkSet        = 0,
            uint32_t         readClkHold       = 0,
            uint32_t         dataMode          = 0,
            uint32_t         prstSel           = 0,
            uint32_t         acqDelay          = 0,
            uint32_t         intTime           = 0,
            uint32_t         digDelay          = 0,
            uint32_t         ampIdle           = 0,
            uint32_t         injTotal          = 0,
            uint32_t         rowColShiftPer    = 0) :
            _shiftSelect(shiftSelect),
            _edgeSelect(edgeSelect),
            _readClkSet(readClkSet),
            _readClkHold(readClkHold),
            _dataMode(dataMode),
            _prstSel(prstSel),
            _acqDelay(acqDelay),
            _intTime(intTime),
            _digDelay(digDelay),
            _ampIdle(ampIdle),
            _injTotal(injTotal),
            _rowColShiftPer(rowColShiftPer) {};

        uint32_t           shiftSelect()        const   { return _shiftSelect;    }
        uint32_t           edgeSelect()         const   { return _edgeSelect;     }
        uint32_t           readClkSet()         const   { return _readClkSet;     }
        uint32_t           readClkHold()        const   { return _readClkHold;    }
        uint32_t           dataMode()           const   { return _dataMode;       }
        uint32_t           prstSel()            const   { return _prstSel;        }
        uint32_t           acqDelay()           const   { return _acqDelay;       }
        uint32_t           intTime()            const   { return _intTime;        }
        uint32_t           digDelay()           const   { return _digDelay;       }
        uint32_t           ampIdle()            const   { return _ampIdle;        }
        uint32_t           injTotal()           const   { return _injTotal;       }
        uint32_t           rowColShiftPer()     const   { return _rowColShiftPer; }
        Pds::CsPad::CsPadReadOnlyCfg&           ro      ()        { return _readOnly;       }
        const Pds::CsPad::CsPadReadOnlyCfg&     ro      ()  const { return _readOnly;       }
        Pds::CsPad::CsPadDigitalPotsCfg&        dp      ()        { return _digitalPots;    }
        const Pds::CsPad::CsPadDigitalPotsCfg&  dp      ()  const { return _digitalPots;    }
        Pds::CsPad::CsPadGainMapCfg*            gm      ()        { return &_gainMap;        }
        const Pds::CsPad::CsPadGainMapCfg*      gm      ()  const { return &_gainMap;        }
        Pds::CsPad::CsPadReadOnlyCfg*           readOnly()        { return &_readOnly;      }
        const Pds::CsPad::CsPadReadOnlyCfg*     readOnly()  const { return &_readOnly;      }

      private:
        uint32_t                     _shiftSelect;
        uint32_t                     _edgeSelect;
        uint32_t                     _readClkSet;
        uint32_t                     _readClkHold;
        uint32_t                     _dataMode;
        uint32_t                     _prstSel;
        uint32_t                     _acqDelay;
        uint32_t                     _intTime;
        uint32_t                     _digDelay;
        uint32_t                     _ampIdle;
        uint32_t                     _injTotal;
        uint32_t                     _rowColShiftPer;

        Pds::CsPad::CsPadReadOnlyCfg     _readOnly;
        Pds::CsPad::CsPadDigitalPotsCfg  _digitalPots;
        Pds::CsPad::CsPadGainMapCfg      _gainMap;
    };

    class ConfigV1
    {
      public:
        static const int Version               = 1;
        ConfigV1() : _testDataIndex(0), _quadMask(0xf) {};
        ConfigV1(uint32_t t, uint32_t m) : _testDataIndex(t), _quadMask(m) {};
        ConfigV1QuadReg*       quads        ()       { return _quads; }
        const ConfigV1QuadReg* quads        () const { return _quads; }
        uint32_t               tdi          () const { return _testDataIndex; }
        uint32_t               quadMask     () const { return _quadMask; }
        uint32_t               runDelay     () const { return _runDelay; }
        uint32_t               eventCode    () const { return _eventCode; }
        uint32_t               inactiveRunMode()const{ return _inactiveRunMode; }
        uint32_t               activeRunMode() const { return _activeRunMode; }
        uint32_t               payloadSize  () const { return _payloadPerQuad; }
        uint32_t               badAsicMask  () const { return _badAsicMask; }
        uint32_t               asicMask     () const { return _AsicMask; }
        uint32_t               numAsicsRead () const { return (_AsicMask&0xf)==1 ? 4 : 16; }
        static const int       version      ()       { return Version; }
      private:
        uint32_t          _runDelay;
        uint32_t          _eventCode;
        uint32_t          _inactiveRunMode;
        uint32_t          _activeRunMode;
        uint32_t          _testDataIndex;
        uint32_t          _payloadPerQuad;
        uint32_t          _badAsicMask;
        uint32_t          _AsicMask;
        uint32_t          _quadMask;
        ConfigV1QuadReg   _quads[MaxQuadsPerSensor];
    };


  } // namespace CsPad

} // namespace Pds 

#pragma pack()

#endif  // CSPAD_CONFIG_V1_HH
