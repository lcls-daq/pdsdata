#include "pdsdata/xtc/BldInfo.hh"
#include "pdsdata/xtc/Level.hh"
#include <stdint.h>
#include <stdio.h>

using namespace Pds;

BldInfo::BldInfo(uint32_t processId, Type type) : Src(Level::Reporter) {
  _log |= processId&0x00ffffff;
  _phy = type;
}

uint32_t BldInfo::processId() const { return _log&0xffffff; }

BldInfo::Type BldInfo::type() const {return (BldInfo::Type)(_phy); }

const char* BldInfo::name(const BldInfo& src){
  static const char* _typeNames[] = {
    "EBeam",
    "PhaseCavity",
    "FEEGasDetEnergy",
    "NH2-SB1-IPM-01",
    "HXX-UM6-IMB-01",
    "HXX-UM6-IMB-02",
    "HFX-DG2-IMB-01",
    "HFX-DG2-IMB-02",
    "XCS-DG3-IMB-03",
    "XCS-DG3-IMB-04",
    "HFX-DG3-IMB-01",
    "HFX-DG3-IMB-02",
    "HXX-DG1-CAM",
    "HFX-DG2-CAM",
    "HFX-DG3-CAM",
    "XCS-DG3-CAM",
    "HFX-MON-CAM"
  };
  return (src.type() < NumberOf ? _typeNames[src.type()] : "-Invalid-");
}
