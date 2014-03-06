#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "pdsdata/xtc/DetInfo.hh"
#include "pdsdata/xtc/ProcInfo.hh"
#include "pdsdata/xtc/XtcIterator.hh"
#include "pdsdata/xtc/XtcFileIterator.hh"
#include "pdsdata/psddl/acqiris.ddl.h"
#include "pdsdata/psddl/ipimb.ddl.h"
#include "pdsdata/psddl/camera.ddl.h"
#include "pdsdata/psddl/evr.ddl.h"
#include "pdsdata/psddl/opal1k.ddl.h"
#include "pdsdata/psddl/pnccd.ddl.h"
#include "pdsdata/psddl/encoder.ddl.h"
#include "pdsdata/psddl/control.ddl.h"
#include "pdsdata/psddl/epics.ddl.h"
#include "pdsdata/psddl/bld.ddl.h"
#include "pdsdata/psddl/princeton.ddl.h"

#include "XtcMonitorClient.hh"

static PNCCD::ConfigV1 cfg;

class myLevelIter : public XtcIterator {
public:
  enum {Stop, Continue};
  myLevelIter(Xtc* xtc, unsigned depth) : XtcIterator(xtc), _depth(depth) {}

  void process(const DetInfo& d, const Camera::FrameV1& f) {
    printf("*** Processing frame object\n");
  }
  void process(const DetInfo&, const Acqiris::DataDescV1&) {
    printf("*** Processing acqiris data object\n");
  }
  void process(const DetInfo&, const Acqiris::ConfigV1&) {
    printf("*** Processing Acqiris config object\n");
  }
  void process(const DetInfo&, const Ipimb::DataV1&) {
    printf("*** Processing ipimb data object\n");
  }
  void process(const DetInfo&, const Ipimb::ConfigV1&) {
    printf("*** Processing Ipimb config object\n");
  }
  void process(const DetInfo&, const Encoder::DataV1&) {
    printf("*** Processing Encoder data object\n");
  }
  void process(const DetInfo&, const Encoder::ConfigV1&) {
    printf("*** Processing Encoder config object\n");
  }
  void process(const DetInfo&, const Opal1k::ConfigV1&) {
    printf("*** Processing Opal1000 config object\n");
  }
  void process(const DetInfo&, const Camera::FrameFexConfigV1&) {
    printf("*** Processing frame feature extraction config object\n");
  }
  void process(const DetInfo&, const Camera::TwoDGaussianV1& o) {
    printf("*** Processing 2DGauss object\n");
  }
  void process(const DetInfo&, const PNCCD::ConfigV1& config) {
    cfg = config;
    printf("*** Processing pnCCD config.  Number of Links: %d, PayloadSize per Link: %d\n",
           cfg.numLinks(),cfg.payloadSizePerLink());
  }
//   void process(const DetInfo& di, PnccdFrameHeaderType* frh) {
//     enum {numberOfLinks=4, payloadPerLink=(1<<19)+16};
//     uint8_t* pb = reinterpret_cast<uint8_t*>(frh);
//     PnccdFrameHeaderType* fp;
//     for (uint32_t i=0; i<numberOfLinks; i++) {
//       fp = reinterpret_cast<PnccdFrameHeaderType*>(pb);
//       printf("\tpnCCD frame: %08X %08X %08X %08X\n", fp->specialWord, fp->frameNumber,
//           fp->TimeStampHi, fp->TimeStampLo);
//       unsigned* pu = (unsigned*)(fp+1);
//       printf("\tdata begins: %08X %08X %08X %08X %08X\n", pu[0], pu[1], pu[2], pu[3], pu[4]);
//       pb += payloadPerLink;
//     }
//   }
  void process(const DetInfo& d, const PNCCD::FrameV1& f) {
    for (unsigned i=0;i<cfg.numLinks();i++) {
      printf("*** Processing pnCCD frame number %x segment %d\n",f.frameNumber(),i);
      printf("  pnCCD frameHeader: %08X, %u, %u, %u\n", f.specialWord(), f.frameNumber(),
          f.timeStampHi(), f.timeStampLo());
      const uint16_t* data = reinterpret_cast<const uint16_t*>(&f+1);
      //      unsigned last  = f.sizeofData(cfg); 
      printf("First data words: 0x%4.4x 0x%4.4x\n",data[0],data[1]);
      //      printf("Last  data words: 0x%4.4x 0x%4.4x\n",data[last-2],data[last-1]);
    }
  }
  void process(const DetInfo&, const ControlData::ConfigV1& config) {
    printf("*** Processing Control config object\n");    
    
    printf( "Control PV Number = %d, Monitor PV Number = %d\n", config.npvControls(), config.npvMonitors() );
    for(unsigned int iPvControl=0; iPvControl < config.npvControls(); iPvControl++) {      
      const Pds::ControlData::PVControl& pvControlCur = config.pvControls()[iPvControl];
      if (pvControlCur.array())
        printf( "%s[%d] = ", pvControlCur.name(), pvControlCur.index() );
      else
        printf( "%s = ", pvControlCur.name() );
      printf( "%lf\n", pvControlCur.value() );
    }
    
    for(unsigned int iPvMonitor=0; iPvMonitor < config.npvMonitors(); iPvMonitor++) {      
      const Pds::ControlData::PVMonitor& pvMonitorCur = config.pvMonitors()[iPvMonitor];
      if (pvMonitorCur.array())
        printf( "%s[%d]  ", pvMonitorCur.name(), pvMonitorCur.index() );
      else
        printf( "%s  ", pvMonitorCur.name() );
      printf( "Low %lf  High %lf\n", pvMonitorCur.loValue(), pvMonitorCur.hiValue() );
    }
          
  }  
  void process(const DetInfo&, const Epics::EpicsPvHeader& epicsPv)
  {    
    printf("*** Processing Epics object\n");
//     epicsPv.printPv();
//     printf( "\n" );
  }
  void process(const DetInfo&, const Bld::BldDataFEEGasDetEnergy& bldData) {
    printf("*** Processing FEEGasDetEnergy object\n");
//     bldData.print();
//     printf( "\n" );    
  }  
  void process(const DetInfo&, const Bld::BldDataEBeamV0& bldData) {
    printf("*** Processing EBeamV0 object\n");
//     bldData.print();
//     printf( "\n" );    
  }  
  void process(const DetInfo&, const Bld::BldDataEBeamV3& bldData) {
    printf("*** Processing EBeam object\n");
//     bldData.print();
//     printf( "\n" );    
  }  
  void process(const DetInfo&, const Bld::BldDataPhaseCavity& bldData) {
    printf("*** Processing PhaseCavity object\n");
//     bldData.print();
//     printf( "\n" );    
  }  
  void process(const DetInfo&, const Bld::BldDataGMDV1& bldData) {
    printf("*** Processing Bld GMD object\n");
//     bldData.print();
//     printf( "\n" );    
  }  
  void process(const DetInfo&, const EvrData::ConfigV1&) {
    printf("*** Processing EVR config V1 object\n");
  }
  void process(const DetInfo&, const EvrData::IOConfigV1&) {
    printf("*** Processing EVR IOconfig V1 object\n");
  }
  void process(const DetInfo&, const EvrData::ConfigV2&) {
    printf("*** Processing EVR config V2 object\n");
  }
  void process(const DetInfo&, const EvrData::ConfigV3&) {
    printf("*** Processing EVR config V3 object\n");
  }
  void process(const DetInfo&, const EvrData::ConfigV4&) {
    printf("*** Processing EVR config V4 object\n");
  }
  void process(const DetInfo&, const EvrData::DataV3& data) {
    printf("*** Processing Evr Data object\n");
    
    printf( "# of Fifo Events: %u\n", data.numFifoEvents() );
    
    for ( unsigned int iEventIndex=0; iEventIndex< data.numFifoEvents(); iEventIndex++ )
    {
      const EvrData::FIFOEvent& event = data.fifoEvents()[iEventIndex];
      printf( "[%02u] Event Code %u  TimeStampHigh 0x%x  TimeStampLow 0x%x\n",
              iEventIndex, event.eventCode(), event.timestampHigh(), event.timestampLow() );
    }
    
    printf( "\n" );    
  }  
  void process(const DetInfo&, const Princeton::ConfigV1&) {
    printf("*** Processing Princeton ConfigV1 object\n");
  }
  void process(const DetInfo&, const Princeton::FrameV1&) {
    printf("*** Processing Princeton FrameV1 object\n");
  }
  int process(Xtc* xtc) {
    unsigned i=_depth; while (i--) printf("  ");
    Level::Type level = xtc->src.level();
    printf("%s level contains: %s: ",Level::name(level), TypeId::name(xtc->contains.id()));
    const DetInfo& info = *(DetInfo*)(&xtc->src);
    if (level==Level::Source) {
      printf("%s %d %s %d",
             DetInfo::name(info.detector()),info.detId(),
             DetInfo::name(info.device()),info.devId());
    } else {
      ProcInfo& info = *(ProcInfo*)(&xtc->src);
      printf("IpAddress 0x%x ProcessId 0x%x",info.ipAddr(),info.processId());
    }
    if (xtc->damage.value()) {
      printf(", damage 0x%x", xtc->damage.value());
    }
    printf("\n");
    switch (xtc->contains.id()) {
    case (TypeId::Id_Xtc) : {
      myLevelIter iter(xtc,_depth+1);
      iter.iterate();
      break;
    }
    default:
      printf("\t%sv%d\n",TypeId::name(xtc->contains.id()),xtc->contains.version());
      break;
    }
    return Continue;
  }
private:
  unsigned _depth;
};

class MyXtcMonitorClient : public XtcMonitorClient {
  public:
    MyXtcMonitorClient() {
    }
    virtual int processDgram(Dgram* dg) {
      printf("%s transition: time 0x%x/0x%x, payloadSize 0x%x, damage 0x%x\n",TransitionId::name(dg->seq.service()),
       dg->seq.stamp().fiducials(),dg->seq.stamp().ticks(),dg->xtc.sizeofPayload(), dg->xtc.damage.value());
      myLevelIter iter(&(dg->xtc),0);
      iter.iterate();
      return 0;
    };
};

void usage(char* progname) {
  fprintf(stderr,"Usage: %s [-t <partitionTag>] [-h]\n", progname);
}

int main(int argc, char* argv[]) {
  int c=0;
  unsigned client=0;
  bool serialized=false;
  const char* partitionTag = 0;
  MyXtcMonitorClient myClient;
  char* endPtr;

  while ((c = getopt(argc, argv, "?ht:c:s")) != -1) {
    switch (c) {
    case '?':
    case 'h':
      usage(argv[0]);
      exit(0);
    case 't':
      partitionTag = optarg;
      break;
    case 'c':
      client = strtoul(optarg,&endPtr,0);
      break;
    case 's':
      serialized = true;
      break;
    default:
      usage(argv[0]);
    }
  }
  if (partitionTag==0)
    usage(argv[0]);
  else
    fprintf(stderr, "myClient returned: %d\n", 
      myClient.run(partitionTag,client,serialized ? client : 0));

  return 1;
}
