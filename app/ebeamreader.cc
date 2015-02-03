
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

#include "pdsdata/xtc/XtcIterator.hh"
#include "pdsdata/xtc/XtcFileIterator.hh"
#include "pdsdata/xtc/Level.hh"
#include "pdsdata/xtc/BldInfo.hh"
#include "pdsdata/psddl/bld.ddl.h"

using namespace Pds;

static const double damaged = -1.e9;
class bldData {
public:
  void reset() {
    ebeamV0  = 0;
    ebeamV1  = 0;
    ebeamV2  = 0;
    ebeamV3  = 0;
    ebeamV4  = 0;
    ebeamV5  = 0;
    ebeamV6  = 0;
    ebeamV7  = 0;
  }
  void dump() const {
    int version = -1;
    if (ebeamV0) version=0;
    if (ebeamV1) version=1;
    if (ebeamV2) version=2;
    if (ebeamV3) version=3;
    if (ebeamV4) version=4;
    if (ebeamV5) version=5;
    if (ebeamV6) version=6;
    if (ebeamV7) version=7;

    printf("V%1d %10d %9d %6d ",
           version,
           seconds,
           nanoseconds,
           pulseId);
    
    if (ebeamV0) printf("%5x\n   % -11g % -10g % -11g % -10g\n   %10s %10s % -11g % -11g\n",
                        ebeamV0->damageMask(),
                        ebeamV0->ebeamL3Energy(),
                        ebeamV0->ebeamCharge(),
                        ebeamV0->ebeamLTUPosX(),
                        ebeamV0->ebeamLTUAngX(),
                        " ",  
                        " ",  
                        ebeamV0->ebeamLTUPosY(),
                        ebeamV0->ebeamLTUAngY());

    if (ebeamV1) printf("%5x\n   % -11g % -10g % -11g % -10g % 10g\n   %10s %10s % -11g % -11g\n",
                        ebeamV1->damageMask(),
                        ebeamV1->ebeamL3Energy(),
                        ebeamV1->ebeamCharge(),
                        ebeamV1->ebeamLTUPosX(),
                        ebeamV1->ebeamLTUAngX(),
                        ebeamV1->ebeamPkCurrBC2(),
                        " ",  
                        " ",  
                        ebeamV1->ebeamLTUPosY(),
                        ebeamV1->ebeamLTUAngY());

    if (ebeamV2) printf("%5x\n   % -11g % -10g % -11g % -10g % 10g % -12g\n   %10s %10s % -11g % -11g\n",
                        ebeamV2->damageMask(),
                        ebeamV2->ebeamL3Energy(),
                        ebeamV2->ebeamCharge(),
                        ebeamV2->ebeamLTUPosX(),
                        ebeamV2->ebeamLTUAngX(),
                        ebeamV2->ebeamPkCurrBC2(),
                        ebeamV2->ebeamEnergyBC2(),
                        " ",  
                        " ",  
                        ebeamV2->ebeamLTUPosY(),
                        ebeamV2->ebeamLTUAngY());

    if (ebeamV3) printf("%5x\n   % -11g % -10g % -11g % -10g % 10g % -12g\n   %10s %10s % -11g % -11g % 10g % -12g\n",
                        ebeamV3->damageMask(),
                        ebeamV3->ebeamL3Energy(),
                        ebeamV3->ebeamCharge(),
                        ebeamV3->ebeamLTUPosX(),
                        ebeamV3->ebeamLTUAngX(),
                        ebeamV3->ebeamPkCurrBC2(),
                        ebeamV3->ebeamEnergyBC2(),
                        " ",  
                        " ",  
                        ebeamV3->ebeamLTUPosY(),
                        ebeamV3->ebeamLTUAngY(),
                        ebeamV3->ebeamPkCurrBC1(),
                        ebeamV3->ebeamEnergyBC1());

    if (ebeamV4) printf("%5x\n   % -11g % -10g % -11g % -10g % 10g % -12g % 10g % 10g\n   %10s %10s % -11g % -11g % 10g % -12g % 10g % 10g\n",
                        ebeamV4->damageMask(),
                        ebeamV4->ebeamL3Energy(),
                        ebeamV4->ebeamCharge(),
                        ebeamV4->ebeamLTUPosX(),
                        ebeamV4->ebeamLTUAngX(),
                        ebeamV4->ebeamPkCurrBC2(),
                        ebeamV4->ebeamEnergyBC2(),
                        ebeamV4->ebeamUndPosX(),
                        ebeamV4->ebeamUndAngX(),
                        " ",  
                        " ",  
                        ebeamV4->ebeamLTUPosY(),
                        ebeamV4->ebeamLTUAngY(),
                        ebeamV4->ebeamPkCurrBC1(),
                        ebeamV4->ebeamEnergyBC1(),
                        ebeamV4->ebeamUndPosY(),
                        ebeamV4->ebeamUndAngY());

    if (ebeamV5) printf("%5x\n   % -11g % -10g % -11g % -10g % 10g % -12g % 10g % 10g % 10g % 12g\n   %10s %10s % -11g % -11g % 10g % -12g % 10g % 10g % 10g\n",
                        ebeamV5->damageMask(),
                        ebeamV5->ebeamL3Energy(),
                        ebeamV5->ebeamCharge(),
                        ebeamV5->ebeamLTUPosX(),
                        ebeamV5->ebeamLTUAngX(),
                        ebeamV5->ebeamPkCurrBC2(),
                        ebeamV5->ebeamEnergyBC2(),
                        ebeamV5->ebeamUndPosX(),
                        ebeamV5->ebeamUndAngX(),
                        ebeamV5->ebeamXTCAVAmpl(),
                        ebeamV5->ebeamDumpCharge(),
                        " ",
                        " ",
                        ebeamV5->ebeamLTUPosY(),
                        ebeamV5->ebeamLTUAngY(),
                        ebeamV5->ebeamPkCurrBC1(),
                        ebeamV5->ebeamEnergyBC1(),
                        ebeamV5->ebeamUndPosY(),
                        ebeamV5->ebeamUndAngY(),
                        ebeamV5->ebeamXTCAVPhase());

    if (ebeamV6) printf("%5x\n   % -11g % -10g % -11g % -10g % 10g % -12g % 10g % 10g % 10g % 12g % -12g\n   %10s %10s % -11g % -11g % 10g % -12g % 10g % 10g % 10g % 12g % -12g\n",
                        ebeamV6->damageMask(),
                        ebeamV6->ebeamL3Energy(),
                        ebeamV6->ebeamCharge(),
                        ebeamV6->ebeamLTUPosX(),
                        ebeamV6->ebeamLTUAngX(),
                        ebeamV6->ebeamPkCurrBC2(),
                        ebeamV6->ebeamEnergyBC2(),
                        ebeamV6->ebeamUndPosX(),
                        ebeamV6->ebeamUndAngX(),
                        ebeamV6->ebeamXTCAVAmpl(),
                        ebeamV6->ebeamDumpCharge(),
                        ebeamV6->ebeamLTU250(),
                        " ",
                        " ",
                        ebeamV6->ebeamLTUPosY(),
                        ebeamV6->ebeamLTUAngY(),
                        ebeamV6->ebeamPkCurrBC1(),
                        ebeamV6->ebeamEnergyBC1(),
                        ebeamV6->ebeamUndPosY(),
                        ebeamV6->ebeamUndAngY(),
                        ebeamV6->ebeamXTCAVPhase(),
                        ebeamV6->ebeamPhotonEnergy(),
                        ebeamV6->ebeamLTU450());

    if (ebeamV7) printf("%5x\n   % -11g % -10g % -11g % -10g % 10g % -12g % 10g % 10g % 10g % 12g % -12g\n   %10s %10s % -11g % -11g % 10g % -12g % 10g % 10g % 10g % 12g % -12g\n",
                        ebeamV7->damageMask(),
                        ebeamV7->ebeamL3Energy(),
                        ebeamV7->ebeamCharge(),
                        ebeamV7->ebeamLTUPosX(),
                        ebeamV7->ebeamLTUAngX(),
                        ebeamV7->ebeamPkCurrBC2(),
                        ebeamV7->ebeamEnergyBC2(),
                        ebeamV7->ebeamUndPosX(),
                        ebeamV7->ebeamUndAngX(),
                        ebeamV7->ebeamXTCAVAmpl(),
                        ebeamV7->ebeamDumpCharge(),
                        ebeamV7->ebeamLTU250(),
                        " ",
                        " ",
                        ebeamV7->ebeamLTUPosY(),
                        ebeamV7->ebeamLTUAngY(),
                        ebeamV7->ebeamPkCurrBC1(),
                        ebeamV7->ebeamEnergyBC1(),
                        ebeamV7->ebeamUndPosY(),
                        ebeamV7->ebeamUndAngY(),
                        ebeamV7->ebeamXTCAVPhase(),
                        ebeamV7->ebeamPhotonEnergy(),
                        ebeamV7->ebeamLTU450());

  }
  void header() const {
    printf("%3s %9s %9s %6s %5s %10s %11s %12s %10s %10s %10s\n",
           "Ver", 
           "seconds",
           "nanosec",
           "pulseId",
           "dmg",
           "LTUAng",
           "PkCurr",
           "Energy",
           "UndPos",
           "UndAng",
           "XTCAV");
    printf("%3s %11s %10s %11s %10s %10s %12s %10s %10s %10s %12s %10s\n",
           " ",
           "L3Enrg[MeV]",
           "Charge[nC]",
           "LTUPosX[mm]",
           "X[mrad]",
           "BC2[A]",
           "BC2[mm]",
           "X[mm]",
           "X[mrad]",
           "Ampl[MV]",
           "DmpChrge[#e-]",
           "LTU250[mm]");
    printf("%26s %11s %10s %10s %12s %10s %10s %10s %10s %10s\n",
           " ",
           "LTUPosY[mm]",
           "Y[mrad]",
           "BC1[A]",
           "BC1[mm]",
           "Y[mm]",
           "Y[mrad]",
           "Phase[deg]",
           "PhotEnrgy[eV]",
           "LTU450[mm]");
  } 
  unsigned                      seconds;
  unsigned                      nanoseconds;
  unsigned                      pulseId;
  const Bld::BldDataEBeamV0*         ebeamV0;
  const Bld::BldDataEBeamV1*         ebeamV1;
  const Bld::BldDataEBeamV2*         ebeamV2;
  const Bld::BldDataEBeamV3*         ebeamV3;
  const Bld::BldDataEBeamV4*         ebeamV4;
  const Bld::BldDataEBeamV5*         ebeamV5;
  const Bld::BldDataEBeamV6*         ebeamV6;
  const Bld::BldDataEBeamV7*         ebeamV7;
};

static bldData bld;

class myLevelIter : public XtcIterator {
public:
  enum {Stop, Continue};
  myLevelIter(Xtc* xtc, unsigned depth) : XtcIterator(xtc), _depth(depth) {}

  int process(Xtc* xtc) {
    if (xtc->contains.id() == TypeId::Id_Xtc) {
      myLevelIter iter(xtc,_depth+1);
      iter.iterate();
    }
    else if (xtc->src.level() == Level::Reporter) {
      const BldInfo& info = static_cast<const BldInfo&>(xtc->src);
      switch(info.type()) {
      case BldInfo::EBeam          :
        if (xtc->contains.version()==0){
          bld.ebeamV0  = reinterpret_cast<const Bld::BldDataEBeamV0*>      (xtc->payload());
        }
        if (xtc->contains.version()==1){
          bld.ebeamV1  = reinterpret_cast<const Bld::BldDataEBeamV1*>      (xtc->payload());
        }
        if (xtc->contains.version()==2){
          bld.ebeamV2  = reinterpret_cast<const Bld::BldDataEBeamV2*>      (xtc->payload());
        }
        if (xtc->contains.version()==3){
          bld.ebeamV3  = reinterpret_cast<const Bld::BldDataEBeamV3*>      (xtc->payload());
        }
        if (xtc->contains.version()==4){
          bld.ebeamV4  = reinterpret_cast<const Bld::BldDataEBeamV4*>      (xtc->payload());
        }
        if (xtc->contains.version()==5){
          bld.ebeamV5  = reinterpret_cast<const Bld::BldDataEBeamV5*>      (xtc->payload());
        }
        if (xtc->contains.version()==6){
          bld.ebeamV6  = reinterpret_cast<const Bld::BldDataEBeamV6*>      (xtc->payload());
        }
        if (xtc->contains.version()==7){
          bld.ebeamV7  = reinterpret_cast<const Bld::BldDataEBeamV7*>      (xtc->payload());
        }
        break;
      default:
        break;
      }
    }
    return Continue;
  }
private:
  unsigned _depth;
};

void usage(char* progname) {
  fprintf(stderr,"Usage: %s -f <filename> -b <begin time> -e <end time> [-h]\n", progname);
  fprintf(stderr,"  time is expressed as YYYYMMDD_HH:MM:SS (UTC);\n");
  fprintf(stderr,"  e.g. \"20091101_14:09:00\"  = Nov 1, 2009, 2:09pm (UTC)\n");
}

bool parse_time(const char* arg, ClockTime& clk)
{
  struct tm t;
  char* r=strptime(optarg, "%Y%m%d_%H:%M:%S", &t);
  if (*r) {
    printf("Error parsing time %s\n",arg);
    return false;
  }
  time_t tt = mktime(&t);
  clk = ClockTime(tt,0);
  return true;
}

int main(int argc, char* argv[]) {
  int c;
  char* xtcname=0;
  int parseErr = 0;
  ClockTime begin(0,0);
  ClockTime end(-1,-1);

  while ((c = getopt(argc, argv, "hf:b:e:")) != -1) {
    switch (c) {
    case 'h':
      usage(argv[0]);
      exit(0);
    case 'f':
      xtcname = optarg;
      break;
    case 'b':
      if (!parse_time(optarg,begin)) parseErr++;
      break;
    case 'e':
      if (!parse_time(optarg,end  )) parseErr++;
      break;
    default:
      parseErr++;
    }
  }

  if (!xtcname || parseErr) {
    usage(argv[0]);
    exit(2);
  }

  int fd = open(xtcname,O_RDONLY | O_LARGEFILE);
  if (fd < 0) {
    perror("Unable to open file %s\n");
    exit(2);
  }

  bld.header();
  XtcFileIterator iter(fd,0x900000);
  Dgram* dg;
  unsigned long long bytes=0;
  while ((dg = iter.next())) {

    if (dg->seq.service() != TransitionId::L1Accept) continue;

    if (!(dg->seq.clock() > begin)) continue;
    if (  dg->seq.clock() > end   ) break;

    bld.reset();
    bld.seconds     = dg->seq.clock().seconds();
    bld.nanoseconds = dg->seq.clock().nanoseconds();
    bld.pulseId     = dg->seq.stamp().fiducials();

    myLevelIter iter(&(dg->xtc),0);
    iter.iterate();
    bytes += sizeof(*dg) + dg->xtc.sizeofPayload();

    bld.dump();
  }

  close(fd);
  return 0;
}
