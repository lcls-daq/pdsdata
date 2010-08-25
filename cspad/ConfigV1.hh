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
    enum {PotsPerQuad=80, TwoByTwosPerQuad=4};
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
          _shiftSelect[1] = shiftSelect[1];
          _shiftSelect[2] = shiftSelect[2];
          _shiftSelect[3] = shiftSelect[3];
          _edgeSelect [0] = edgeSelect [0];
          _edgeSelect [1] = edgeSelect [1];
          _edgeSelect [2] = edgeSelect [2];
          _edgeSelect [3] = edgeSelect [3];
        };

        const uint32_t*    shiftSelect()        const   { return _shiftSelect; }
        const uint32_t*    edgeSelect()         const   { return _edgeSelect;  }
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
        Pds::CsPad::CsPadGainMapCfg*            gm      ()        { return &_gainMap;       }
        const Pds::CsPad::CsPadGainMapCfg*      gm      ()  const { return &_gainMap;       }
        Pds::CsPad::CsPadReadOnlyCfg*           readOnly()        { return &_readOnly;      }
        const Pds::CsPad::CsPadReadOnlyCfg*     readOnly()  const { return &_readOnly;      }

      private:
        uint32_t                     _shiftSelect[TwoByTwosPerQuad];
        uint32_t                     _edgeSelect[TwoByTwosPerQuad];
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
        ConfigV1() {};
        ConfigV1(
            uint32_t runDelay,
            uint32_t eventCode,
            uint32_t inactiveRunMode,
            uint32_t activeRunMode,
            uint32_t testDataIndex,
            uint32_t payloadPerQuad,
            uint32_t badAsicMask0,
            uint32_t badAsicMask1,
            uint32_t AsicMask,
            uint32_t quadMask) :
          _runDelay(runDelay),
          _eventCode(eventCode),
          _inactiveRunMode(inactiveRunMode),
          _activeRunMode(activeRunMode),
          _testDataIndex(),
          _payloadPerQuad(payloadPerQuad),
          _badAsicMask0(badAsicMask0),
          _badAsicMask1(badAsicMask1),
          _AsicMask(AsicMask),
          _quadMask(quadMask) {};

        ConfigV1QuadReg*       quads        ()       { return _quads; }
        const ConfigV1QuadReg* quads        () const { return _quads; }
        uint32_t               tdi          () const { return _testDataIndex; }
        uint32_t               quadMask     () const { return _quadMask; }
        uint32_t               runDelay     () const { return _runDelay; }
        uint32_t               eventCode    () const { return _eventCode; }
        uint32_t               inactiveRunMode()const{ return _inactiveRunMode; }
        uint32_t               activeRunMode() const { return _activeRunMode; }
        uint32_t               payloadSize  () const { return _payloadPerQuad; }
        uint32_t               badAsicMask0 () const { return _badAsicMask0; }
        uint32_t               badAsicMask1 () const { return _badAsicMask1; }
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
        uint32_t          _badAsicMask0;
        uint32_t          _badAsicMask1;
        uint32_t          _AsicMask;  // there are a maximum of 64 ASICs
        uint32_t          _quadMask;
        ConfigV1QuadReg   _quads[MaxQuadsPerSensor];
    };


  } // namespace CsPad

} // namespace Pds 

#pragma pack()

#endif  // CSPAD_CONFIG_V1_HH
