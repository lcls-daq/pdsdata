
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
    gasdet   = 0;
    gasdetV1 = 0;
    ebeamV0  = 0;
    ebeamV1  = 0;
    ebeamV2  = 0;
    ebeamV3  = 0;
    ebeamV4  = 0;
    ebeamV5  = 0;
    ebeamV6  = 0;
    phasecav = 0;
    gmdV0    = 0;
    gmdV1    = 0;
    gmdV2    = 0;
  }
  void dump() const {
    printf("%d\t%d\t%d\t",
     seconds,
     nanoseconds,
     pulseId);

    if (gasdet)   printf("%g\t%g\t%g\t%g\t",
                         gasdet->f_11_ENRC(),
                         gasdet->f_12_ENRC(),
                         gasdet->f_21_ENRC(),
                         gasdet->f_22_ENRC());
    else if (gasdetV1)   printf("%g\t%g\t%g\t%g\t%g\t%g\t",
                                gasdetV1->f_11_ENRC(),
                                gasdetV1->f_12_ENRC(),
                                gasdetV1->f_21_ENRC(),
                                gasdetV1->f_22_ENRC(),
                                gasdetV1->f_63_ENRC(),
                                gasdetV1->f_64_ENRC());
    else          printf("%g\t%g\t%g\t%g\t",
                         damaged,
                         damaged,
                         damaged,
                         damaged);

    if (ebeamV0) printf("%g\t%g\t%g\t%g\t%g\t%g\t",
                        ebeamV0->ebeamCharge(),
                        ebeamV0->ebeamL3Energy(),
                        ebeamV0->ebeamLTUPosX(),
                        ebeamV0->ebeamLTUPosY(),
                        ebeamV0->ebeamLTUAngX(),
                        ebeamV0->ebeamLTUAngY());
    if (ebeamV1) printf("%g\t",
                        ebeamV1->ebeamPkCurrBC2());
    if (ebeamV2) printf("%g\t",
                        ebeamV2->ebeamEnergyBC2());
    if (ebeamV3) printf("%g\t%g\t",
                        ebeamV3->ebeamPkCurrBC1(),
                        ebeamV3->ebeamEnergyBC1());
    if (ebeamV4) printf("%g\t%g\t%g\t%g\t",
                        ebeamV4->ebeamUndPosX(),
                        ebeamV4->ebeamUndPosY(),
                        ebeamV4->ebeamUndAngX(),
                        ebeamV4->ebeamUndAngY());
    if (ebeamV5) printf("%g\t%g\t%g\t",
                        ebeamV5->ebeamXTCAVAmpl(),
                        ebeamV5->ebeamXTCAVPhase(),
                        ebeamV5->ebeamDumpCharge());
    if (ebeamV6) printf("%g\t%g\t%g\t",
                        ebeamV6->ebeamPhotonEnergy(),
                        ebeamV6->ebeamLTU250(),
                        ebeamV6->ebeamLTU450());

    if (phasecav) printf("%g\t%g\t%g\t%g\n",
                         phasecav->fitTime1(),
                         phasecav->fitTime2(),
                         phasecav->charge1(),
                         phasecav->charge2());
    else          printf("%g\t%g\t%g\t%g\n",
                         damaged,
                         damaged,
                         damaged,
                         damaged);

    //    if (gmd) gmd->print();
  }
  void header() const {
    static const char* headers[] = { "seconds",
                                     "nanoseconds",
                                     "pulseId",
                                     "GDET:FEE:241:ENRC[mJ]",
                                     "GDET:FEE:242:ENRC[mJ]",
                                     "GDET:FEE:361:ENRC[mJ]",
                                     "GDET:FEE:362:ENRC[mJ]",
                                     "GDET:FEE:363:ENRC[mJ]",
                                     "GDET:FEE:364:ENRC[mJ]",
                                     "ebeamCharge[nC]",
                                     "ebeamL3Energy[MeV]",
                                     "ebeamLTUPosX[mm]",
                                     "ebeamLTUPosY[mm]",
                                     "ebeamLTUAngX[mrad]",
                                     "ebeamLTUAngY[mrad]",
                                     "ebeamPkCurrBC2[Amp]",
                                     "ebeamBC2Energy[mm]",
                                     "ebeamPkCurrBC1[Amp]",
                                     "ebeamBC1Energy[mm]",
                                     "PhCav:FitTime1[ps]",
                                     "PhCav:FitTime2[ps]",
                                     "PhCav:Charge1[pC]",
                                     "PhCav:Charge2[pC]",
                                     NULL };
    for(const char** h = headers; *h != NULL; h++)
      printf("%s\t",*h);
    printf("\n");
  }
  unsigned                      seconds;
  unsigned                      nanoseconds;
  unsigned                      pulseId;
  const Bld::BldDataFEEGasDetEnergy*   gasdet;
  const Bld::BldDataFEEGasDetEnergyV1* gasdetV1;
  const Bld::BldDataEBeamV0*         ebeamV0;
  const Bld::BldDataEBeamV1*         ebeamV1;
  const Bld::BldDataEBeamV2*         ebeamV2;
  const Bld::BldDataEBeamV3*         ebeamV3;
  const Bld::BldDataEBeamV4*         ebeamV4;
  const Bld::BldDataEBeamV5*         ebeamV5;
  const Bld::BldDataEBeamV6*         ebeamV6;
  const Bld::BldDataPhaseCavity*     phasecav;
  const Bld::BldDataGMDV0*             gmdV0;
  const Bld::BldDataGMDV1*             gmdV1;
  const Bld::BldDataGMDV2*             gmdV2;
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
    else if (xtc->damage.value())
      ;
    else if (xtc->src.level() == Level::Reporter) {
      const BldInfo& info = static_cast<const BldInfo&>(xtc->src);
      switch(info.type()) {
      case BldInfo::EBeam          :
        if (xtc->contains.version()>=0)
          bld.ebeamV0  = reinterpret_cast<const Bld::BldDataEBeamV0*>      (xtc->payload());
        if (xtc->contains.version()>=1)
          bld.ebeamV1  = reinterpret_cast<const Bld::BldDataEBeamV1*>      (xtc->payload());
        if (xtc->contains.version()>=2)
          bld.ebeamV2  = reinterpret_cast<const Bld::BldDataEBeamV2*>      (xtc->payload());
        if (xtc->contains.version()>=3)
          bld.ebeamV3  = reinterpret_cast<const Bld::BldDataEBeamV3*>      (xtc->payload());
        if (xtc->contains.version()>=5)
          bld.ebeamV5  = reinterpret_cast<const Bld::BldDataEBeamV5*>      (xtc->payload());
        if (xtc->contains.version()>=6)
          bld.ebeamV6  = reinterpret_cast<const Bld::BldDataEBeamV6*>      (xtc->payload());
        break;
      case BldInfo::PhaseCavity    :
        bld.phasecav = reinterpret_cast<const Bld::BldDataPhaseCavity*>    (xtc->payload());
        break;
      case BldInfo::FEEGasDetEnergy:
        switch(xtc->contains.version()) {
        case 0:
          bld.gasdet   = reinterpret_cast<const Bld::BldDataFEEGasDetEnergy*>(xtc->payload());
          break;
        case 1:
          bld.gasdetV1 = reinterpret_cast<const Bld::BldDataFEEGasDetEnergyV1*>(xtc->payload());
          break;
        default:
          break;
        }
      case BldInfo::GMD:
        switch(xtc->contains.version()) {
        case 0:
          bld.gmdV0     = reinterpret_cast<const Bld::BldDataGMDV0*>(xtc->payload());
          break;
        case 1:
          bld.gmdV1     = reinterpret_cast<const Bld::BldDataGMDV1*>(xtc->payload());
          break;
        case 2:
          bld.gmdV2     = reinterpret_cast<const Bld::BldDataGMDV2*>(xtc->payload());
          break;
        default:
          break;
        }
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
