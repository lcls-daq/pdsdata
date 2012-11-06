#ifndef Pds_BldInfo_hh
#define Pds_BldInfo_hh

#include <stdint.h>
#include "pdsdata/xtc/Src.hh"

namespace Pds {

  class Node;

  class BldInfo : public Src {
  public:

    enum Type { EBeam            = 0, 
                PhaseCavity      = 1, 
                FEEGasDetEnergy  = 2,
                Nh2Sb1Ipm01      = 3,  
                HxxUm6Imb01      = 4, 
                HxxUm6Imb02      = 5,
                HfxDg2Imb01      = 6, 
                HfxDg2Imb02      = 7,
                XcsDg3Imb03      = 8, 
                XcsDg3Imb04      = 9,
                HfxDg3Imb01      = 10, 
                HfxDg3Imb02      = 11,
                HxxDg1Cam        = 12,   
                HfxDg2Cam        = 13,
                HfxDg3Cam        = 14,   
                XcsDg3Cam        = 15,
                HfxMonCam        = 16,
                HfxMonImb01      = 17, 
                HfxMonImb02      = 18,
                HfxMonImb03      = 19,
                MecLasEm01       = 20, 
                MecTctrPip01     = 21,
                MecTcTrDio01     = 22,
                MecXt2Ipm02      = 23, 
                MecXt2Ipm03      = 24, 
                MecHxmIpm01      = 25,
                GMD              = 26,
                NumberOf };

    BldInfo() {}
    BldInfo(uint32_t processId,
            Type     type);

    uint32_t processId() const;
    Type     type()  const;

    static const char* name(const BldInfo&);
  };

}
#endif
