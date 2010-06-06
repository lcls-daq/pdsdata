#ifndef Pds_DetInfo_hh
#define Pds_DetInfo_hh

#include <stdint.h>
#include "pdsdata/xtc/Src.hh"

namespace Pds {

  class Node;

  class DetInfo : public Src {
  public:
    /*
     * Notice: New enum values should be appended to the end of the enum list, since
     *   the old values have already been recorded in the existing xtc files. 
     */
    enum Detector {NoDetector,AmoIms,AmoGasdet,AmoETof,AmoITof,AmoMbes,AmoVmi,AmoBps,
                   Camp,EpicsArch,BldEb,
		   SxrBeamline,SxrEndstation,
		   XppSb1Ipm             ,XppSb1Pim,XppMonPim,XppSb2Ipm,XppSb3Ipm,XppSb3Pim,XppSb4Pim,
		   NumDetector};
    enum Device   {NoDevice,Evr,Acqiris,Opal1000,TM6740,pnCCD,Princeton,Fccd,Ipimb,Encoder,NumDevice};

    DetInfo() {}
    DetInfo(uint32_t processId,
            Detector det, uint32_t detId,
            Device dev,   uint32_t devId);

    bool operator==(const DetInfo&) const;

    uint32_t processId() const;
    Detector detector()  const;
    Device   device()    const;
    uint32_t detId()     const;
    uint32_t devId()     const;

    static const char* name(Detector);
    static const char* name(Device);
    static const char* name(const DetInfo&);
  };

}
#endif
