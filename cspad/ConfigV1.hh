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
    enum RunModes  {NoRunning=0, RunButDrop=1, RunAndSendToRCE=2, RunAndSendTriggeredByTTL=3, NumberOfRunModes=4 };
    enum DataModes {normal=0, shiftTest=1, testData=2, reserved=3};

    class CsPadDigitalPotsCfg
    {
      public:
        CsPadDigitalPotsCfg() {int i = 0; while (i<PotsPerQuad) pots[i++] = 0;}
        uint8_t         value(unsigned i) {return pots[i];}
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
        GainMap*     map()    { return &_gainMap; }
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

        uint32_t           shiftSelect()                { return _shiftSelect;    }
        uint32_t           edgeSelect()                 { return _edgeSelect;     }
        uint32_t           readClkSet()                 { return _readClkSet;     }
        uint32_t           readClkHold()                { return _readClkHold;    }
        uint32_t           dataMode()                   { return _dataMode;       }
        uint32_t           prstSel()                    { return _prstSel;        }
        uint32_t           acqDelay()                   { return _acqDelay;       }
        uint32_t           intTime()                    { return _intTime;        }
        uint32_t           digDelay()                   { return _digDelay;       }
        uint32_t           ampIdle()                    { return _ampIdle;        }
        uint32_t           injTotal()                   { return _injTotal;       }
        uint32_t           rowColShiftPer()             { return _rowColShiftPer; }
        Pds::CsPad::CsPadReadOnlyCfg&     ro()          { return _readOnly;       }
        Pds::CsPad::CsPadDigitalPotsCfg&  dp()          { return _digitalPots;    }
        Pds::CsPad::CsPadGainMapCfg*      gm()          { return &_gainMap;        }
        Pds::CsPad::CsPadReadOnlyCfg*     readOnly()    { return &_readOnly;      }

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
        Pds::CsPad::ConfigV1QuadReg*    quads()         { return _quads; }
        uint32_t                        tdi()           { return _testDataIndex; }
        uint32_t                        quadMask()      { return _quadMask; }
        uint32_t                        runDelay()      { return _runDelay; }
        uint32_t                        eventCode()     { return _eventCode; }
        uint32_t                        activeRunMode() { return _activeRunMode; }
        uint32_t                        payloadSize()   { return _payloadPerQuad; }
        uint32_t                        asicMask()      { return _AsicMask; }
        static const int                version()       { return Version; }
      private:
        uint32_t                      _runDelay;
        uint32_t                      _eventCode;
        uint32_t                      _activeRunMode;
        uint32_t                      _testDataIndex;
        uint32_t                      _payloadPerQuad;
        uint32_t                      _badAsicMask;
        uint32_t                      _AsicMask;
        uint32_t                      _quadMask;
        Pds::CsPad::ConfigV1QuadReg   _quads[MaxQuadsPerSensor];
    };


  } // namespace CsPad

} // namespace Pds 

#pragma pack()

#endif  // CSPAD_CONFIG_V1_HH
