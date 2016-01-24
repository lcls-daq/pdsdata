#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <map>
#include <string>

#include "pdsdata/xtc/DetInfo.hh"
#include "pdsdata/xtc/ProcInfo.hh"
#include "pdsdata/xtc/XtcIterator.hh"
#include "pdsdata/xtc/XtcFileIterator.hh"
#include "pdsdata/psddl/acqiris.ddl.h"
#include "pdsdata/psddl/ipimb.ddl.h"
#include "pdsdata/psddl/encoder.ddl.h"
#include "pdsdata/psddl/camera.ddl.h"
#include "pdsdata/psddl/fccd.ddl.h"
#include "pdsdata/psddl/timepix.ddl.h"
#include "pdsdata/psddl/opal1k.ddl.h"
#include "pdsdata/psddl/pulnix.ddl.h"
#include "pdsdata/psddl/pnccd.ddl.h"
#include "pdsdata/psddl/evr.ddl.h"
#include "pdsdata/psddl/control.ddl.h"
#include "pdsdata/psddl/epics.ddl.h"
#include "pdsdata/psddl/bld.ddl.h"
#include "pdsdata/psddl/princeton.ddl.h"
#include "pdsdata/psddl/cspad.ddl.h"
#include "pdsdata/psddl/cspad2x2.ddl.h"
#include "pdsdata/psddl/lusi.ddl.h"
#include "pdsdata/psddl/alias.ddl.h"
#include "pdsdata/psddl/rayonix.ddl.h"
#include "pdsdata/psddl/smldata.ddl.h"
#include "pdsdata/psddl/partition.ddl.h"

static unsigned eventCount = 0;

using namespace Pds;
using std::map;
using std::string;
using Pds::Alias::SrcAlias;

static map<Src,string> aliasMap;

class myLevelIter : public XtcIterator {
public:
  enum {Stop, Continue};
  myLevelIter(Xtc* xtc, unsigned depth, long long int lliOffset) : XtcIterator(xtc), _depth(depth), _lliOffset(lliOffset) {}

  void process(const DetInfo& d, const Camera::FrameV1& f) {
    printf("*** Processing frame object\n");
  }
  void process(const DetInfo&, const Acqiris::DataDescV1&) {
    printf("*** Processing acqiris data object\n");
  }
  void process(const DetInfo&, const Acqiris::ConfigV1&) {
    printf("*** Processing Acqiris config object\n");
  }
  void process(const DetInfo& i, const Acqiris::TdcDataV1& d, uint32_t payloadSize) {
    printf("*** Processing acqiris TDC data object for %s\n",
     DetInfo::name(i));
    unsigned nItems = payloadSize / sizeof(Acqiris::TdcDataV1_Item);
    ndarray<const Acqiris::TdcDataV1_Item,1> a = d.data(nItems);
    for(unsigned j=0; j<a.shape()[0]; j++) {
      if (a[j].source() == Acqiris::TdcDataV1_Item::AuxIO &&
          static_cast<const Acqiris::TdcDataV1Marker&>(a[j]).type() < Acqiris::TdcDataV1Marker::AuxIOMarker) 
        break;
      switch(a[j].source()) {
      case Acqiris::TdcDataV1_Item::Comm:
        printf("common start %d\n",
               static_cast<const Acqiris::TdcDataV1Common&>(a[j]).nhits());
        break;
      case Acqiris::TdcDataV1_Item::AuxIO:
        break;
      default:
        { 
          const Acqiris::TdcDataV1Channel& c = 
            static_cast<const Acqiris::TdcDataV1Channel&>(a[j]);
          if (!c.overflow())
            printf("ch %d : %g ns\n",
                   a[j].source()-1, c.time());
          break;
        }
      }
    }
  }
  void process(const DetInfo& i, const Acqiris::TdcConfigV1& c) {
    printf("*** Processing Acqiris TDC config object for %s\n",
     DetInfo::name(i));
    for(unsigned j=0; j<Acqiris::TdcConfigV1::NChannels; j++) {
      const Acqiris::TdcChannel& ch = c.channels()[j];
      printf("chan %d : %s, slope %c, level %gv\n",
             ch.channel(),
             ch.mode ()==Acqiris::TdcChannel::Inactive?"inactive":"active",
             ch.slope()==Acqiris::TdcChannel::Positive?'+':'-',
             ch.level());
    }
  }
  void process(const DetInfo&, const Ipimb::DataV1&) {
    printf("*** Processing ipimb data object\n");
  }
  void process(const DetInfo&, const Ipimb::ConfigV1&) {
    printf("*** Processing Ipimb config object\n");
  }
  void process(const DetInfo&, const Ipimb::DataV2&) {
    printf("*** Processing ipimb data object\n");
  }
  void process(const DetInfo&, const Ipimb::ConfigV2&) {
    printf("*** Processing Ipimb config object\n");
  }
  void process(const DetInfo&, const Encoder::DataV1&) {
    printf("*** Processing encoder DataV1 object\n");
  }
  void process(const DetInfo&, const Encoder::DataV2&) {
    printf("*** Processing encoder DataV2 object\n");
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
  void process(const DetInfo&, const FCCD::FccdConfigV1&) {
    printf("*** Processing FCCD ConfigV1 object\n");
  }
  void process(const DetInfo&, const FCCD::FccdConfigV2&) {
    printf("*** Processing FCCD ConfigV2 object\n");
  }
  void process(const DetInfo&, const Timepix::ConfigV1&) {
    printf("*** Processing Timepix ConfigV1 object\n");
  }
  void process(const DetInfo&, const Timepix::ConfigV2&) {
    printf("*** Processing Timepix ConfigV2 object\n");
  }
  void process(const DetInfo&, const Timepix::ConfigV3&) {
    printf("*** Processing Timepix ConfigV3 object\n");
  }
  void process(const DetInfo&, const Timepix::DataV1&) {
    printf("*** Processing Timepix DataV1 object\n");
  }
  void process(const DetInfo&, const Timepix::DataV2&) {
    printf("*** Processing Timepix DataV2 object\n");
  }
  void process(const DetInfo&, const Camera::TwoDGaussianV1& o) {
    printf("*** Processing 2DGauss object\n");
  }
  void process(const DetInfo& det, const PNCCD::ConfigV1& config) {
    if ( det.detId() != 0 )
    {
      printf( "myLevelIter::process(...,PNCCD::ConfigV1&): pnCCD detector Id (%d) is not 0\n", det.detId() );
      return;
    }
    if ( det.devId() < 0 || det.devId() > 1)
    {
      printf( "myLevelIter::process(...,PNCCD::ConfigV1&): pnCCD device Id (%d) is out of range (0..1)\n", det.devId() );
      return;
    }
    
    _pnCcdCfgListV1[det.devId()] = config;
    printf("*** Processing pnCCD config.  Number of Links: %d, PayloadSize per Link: %d\n",
           config.numLinks(),config.payloadSizePerLink());
  }  
  void process(const DetInfo& det, const PNCCD::ConfigV2& config) {
    if ( det.detId() != 0 )
    {
      printf( "myLevelIter::process(...,PNCCD::ConfigV2&): pnCCD detector Id (%d) is not 0\n", det.detId() );
      return;
    }
    if ( det.devId() < 0 || det.devId() > 1)
    {
      printf( "myLevelIter::process(...,PNCCD::ConfigV2&): pnCCD device Id (%d) is out of range (0..1)\n", det.devId() );
      return;
    }

    _pnCcdCfgListV2[det.devId()] = config;
    printf("*** Processing pnCCD config.  Number of Links: %u, PayloadSize per Link: %u\n",
           config.numLinks(),config.payloadSizePerLink());
    printf("\tNumber of Channels %u, Number of Rows %u, Number of SubModule Channels %u\n\tNumber of SubModule Rows %u, Number of SubModules, %u\n",
        config.numChannels(),config.numRows(), config.numSubmoduleChannels(),config.numSubmoduleRows(),config.numSubmodules());
    printf("\tCamex Magic 0x%x, info %s, Timing File Name %s\n", config.camexMagic(),config.info(),config.timingFName());
  }
  void process(const DetInfo& det, const PNCCD::FrameV1* f) {
    if ( det.detId() != 0 )
    {
      printf( "myLevelIter::process(...,PNCCD::FrameV1*): pnCCD detector Id (%d) is not 0\n", det.detId() );
      return;
    }
    if ( det.devId() < 0 || det.devId() > 1)
    {
      printf( "myLevelIter::process(...,PNCCD::FrameV1*): pnCCD device Id (%d) is out of range (0..1)\n", det.devId() );
      return;
    }
    
    printf("*** Processing pnCCD Frame\n");
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
  void process(const DetInfo&, const ControlData::ConfigV2& config) {
    printf("*** Processing Control config object\n");    
    
    printf( "Control PV Number = %d, Monitor PV Number = %d, Label PV Number = %d\n", config.npvControls(), config.npvMonitors(), config.npvLabels() );
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
          
    for(unsigned int iPvLabel=0; iPvLabel < config.npvLabels(); iPvLabel++) {      
      const Pds::ControlData::PVLabel& pvLabelCur = config.pvLabels()[iPvLabel];
      printf( "%s = %s\n", pvLabelCur.name(), pvLabelCur.value() );
    }
          
  } 
  void process(const DetInfo&, const ControlData::ConfigV3& config) {
    printf("*** Processing Control config object\n");    
    
    printf( "Control PV Number = %d, Monitor PV Number = %d, Label PV Number = %d\n", config.npvControls(), config.npvMonitors(), config.npvLabels() );
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
          
    for(unsigned int iPvLabel=0; iPvLabel < config.npvLabels(); iPvLabel++) {      
      const Pds::ControlData::PVLabel& pvLabelCur = config.pvLabels()[iPvLabel];
      printf( "%s = %s\n", pvLabelCur.name(), pvLabelCur.value() );
    }
          
  } 

  void process(const DetInfo&, const Epics::EpicsPvHeader& epicsPv)
  {    
    printf("*** Processing Epics object\n");
    //    epicsPv.printPv();
    printf( "\n" );
  }
  void process(const DetInfo&, const Bld::BldDataFEEGasDetEnergy& bldData) {
    printf("*** Processing FEEGasDetEnergy object\n");
//     bldData.print();
//     printf( "\n" );    
  }  
  void process(const DetInfo&, const Bld::BldDataFEEGasDetEnergyV1& bldData) {
    printf("*** Processing FEEGasDetEnergyV1 object\n");
//     bldData.print();
//     printf( "\n" );    
  }  
  void process(const DetInfo&, const Bld::BldDataEBeamV0& bldData) {
    printf("*** Processing EBeamV0 object\n");
//     bldData.print();
//     printf( "\n" );    
  }  
  void process(const DetInfo&, const Bld::BldDataEBeamV1& bldData) {
    printf("*** Processing EBeamV1 object\n");
//     bldData.print();
//     printf( "\n" );    
  }  
  void process(const DetInfo&, const Bld::BldDataEBeamV2& bldData) {
    printf("*** Processing EBeamV2 object\n");
//     bldData.print();
//     printf( "\n" );    
  }  
  void process(const DetInfo&, const Bld::BldDataEBeamV3& bldData) {
    printf("*** Processing EBeamV3 object\n");
//     bldData.print();
//     printf( "\n" ); 
  }   
  void process(const DetInfo&, const Bld::BldDataEBeamV4& bldData) {
    printf("*** Processing EBeamV4 object\n");
//     bldData.print();
//     printf( "\n" );    
  }
  void process(const DetInfo&, const Bld::BldDataEBeamV5& bldData) {
    printf("*** Processing EBeamV5 object\n");
//     bldData.print();
//     printf( "\n" );    
  }
  void process(const DetInfo&, const Bld::BldDataEBeamV6& bldData) {
    printf("*** Processing EBeamV6 object\n");
//     bldData.print();
//     printf( "\n" );    
  }  
  void process(const DetInfo&, const Bld::BldDataPhaseCavity& bldData) {
    printf("*** Processing PhaseCavity object\n");
//     bldData.print();
//     printf( "\n" );    
  } 
  void process(const DetInfo&, const Bld::BldDataIpimbV0& bldData) {
    printf("*** Processing Bld-Ipimb V0 object\n");
//     bldData.print();
//     printf( "\n" );    
  } 
  void process(const DetInfo&, const Bld::BldDataIpimbV1& bldData) {
    printf("*** Processing Bld-Ipimb V1 object\n");
//     bldData.print();
//     printf( "\n" );    
  } 
  
  void process(const DetInfo&, const Bld::BldDataGMDV0& bldData) {
    printf("*** Processing Bld-GMD V0 object\n");
//     bldData.print();
//     printf( "\n" );    
  } 

  void process(const DetInfo&, const Bld::BldDataGMDV1& bldData) {
    printf("*** Processing Bld-GMD V1 object\n");
//     bldData.print();
//     printf( "\n" );
  }
  
  void process(const DetInfo&, const EvrData::IOConfigV1&) {
    printf("*** Processing EVR IOconfig V1 object\n");
  }
  void process(const DetInfo&, const EvrData::ConfigV1&) {
    printf("*** Processing EVR config V1 object\n");
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
  void process(const DetInfo&, const EvrData::ConfigV5&) {
    printf("*** Processing EVR config V5 object\n");
  }
  void process(const DetInfo&, const EvrData::ConfigV6&) {
    printf("*** Processing EVR config V6 object\n");
  }
  void process(const DetInfo&, const EvrData::ConfigV7&) {
    printf("*** Processing EVR config V7 object\n");
  }
  void process(const DetInfo&, const EvrData::DataV3& data) {
    printf("*** Processing Evr Data object\n");
    eventCount++;    

    printf( "# of Fifo Events: %u\n", data.numFifoEvents() );
    for ( unsigned int iEventIndex=0; iEventIndex< data.numFifoEvents(); iEventIndex++ ) {
      const EvrData::FIFOEvent& event = data.fifoEvents()[iEventIndex];
      printf( "[%02u] Event Code %u  TimeStampHigh 0x%x  TimeStampLow 0x%x\n",
              iEventIndex, event.eventCode(), event.timestampHigh(), event.timestampLow() );
      if (event.eventCode() == 162)
        printf ("Blank shot eventcode 162 found at eventNo: %u \n",eventCount); 
      if (event.eventCode() == 163)
        printf ("Blank shot eventcode 163 found at eventNo: %u \n",eventCount); 
    }    
    printf( "\n" );    
  }  
  void process(const DetInfo&, const Princeton::ConfigV1&) {
    printf("*** Processing Princeton ConfigV1 object\n");
  }
  void process(const DetInfo&, const Princeton::FrameV1&) {
    printf("*** Processing Princeton FrameV1 object\n");
  }
  void process(const DetInfo&, const Princeton::InfoV1&) {
    printf("*** Processing Princeton InfoV1 object\n");
  }
  void process(const DetInfo&, const CsPad2x2::ElementV1&) {
    printf("*** Processing CsPad2x2 ElementV1 object\n");
  }
  void process(const DetInfo&, const CsPad::ElementV1&) {
    printf("*** Processing CsPad ElementV1 object\n");
  }
  void process(const DetInfo&, const CsPad::ConfigV1&) {
    printf("*** Processing CsPad ElementV1 object\n");
  }
  void process(const DetInfo&, const Lusi::IpmFexConfigV1&) {
    printf("*** Processing LUSI IpmFexConfigV1 object\n");
  }
  void process(const DetInfo&, const Lusi::IpmFexConfigV2&) {
    printf("*** Processing LUSI IpmFexConfigV2 object\n");
  }
  void process(const DetInfo&, const Lusi::IpmFexV1&) {
    printf("*** Processing LUSI IpmFexV1 object\n");
  }
  void process(const DetInfo&, const Lusi::DiodeFexConfigV1&) {
    printf("*** Processing LUSI DiodeFexConfigV1 object\n");
  }
  void process(const DetInfo&, const Lusi::DiodeFexConfigV2&) {
    printf("*** Processing LUSI DiodeFexConfigV2 object\n");
  }
  void process(const DetInfo&, const Lusi::DiodeFexV1&) {
    printf("*** Processing LUSI DiodeFexV1 object\n");
  }
  void process(const DetInfo &, const Lusi::PimImageConfigV1 &)
  {
    printf("*** Processing LUSI PimImageConfigV1 object\n");
  }  
  void process(const DetInfo &, const Pulnix::TM6740ConfigV1 &)
  {
    printf("*** Processing Pulnix TM6740ConfigV1 object\n");
  }
  void process(const DetInfo &, const Pulnix::TM6740ConfigV2 &)
  {
    printf("*** Processing Pulnix::TM6740ConfigV2 object\n");
  }  
  void process(const DetInfo &, const Alias::ConfigV1 &aliasConfig)
  {
    printf("*** Processing Alias ConfigV1 object\n");
    ndarray<const SrcAlias,1> a = aliasConfig.srcAlias();
    for(const SrcAlias* p = a.begin(); p!=a.end(); p++) {
      printf("\t%08x.%08x\t%s\n",p->src().log(),p->src().phy(),p->aliasName());
      aliasMap[p->src()] = string(p->aliasName(), SrcAlias::AliasNameMax);
    }      
  }
  void process(const DetInfo &, const Rayonix::ConfigV2 &rayonixConfig)
  {
    char lilbuf[Rayonix::ConfigV2::DeviceIDMax+20];
    printf("*** Processing Rayonix ConfigV2 object\n");
    snprintf(lilbuf, sizeof(lilbuf), "deviceID: '%s'", rayonixConfig.deviceID());
    printf("\t%s\n\treadoutMode: ", lilbuf);
    switch (rayonixConfig.readoutMode()) {
      case Rayonix::ConfigV2::Unknown:  printf("Unknown"); break;
      case Rayonix::ConfigV2::Standard: printf("Standard"); break;
      case Rayonix::ConfigV2::HighGain: printf("HighGain"); break;
      case Rayonix::ConfigV2::LowNoise: printf("LowNoise"); break;
      case Rayonix::ConfigV2::HDR: printf("HDR"); break;
      default: printf("Unrecognized (%d)", (int)rayonixConfig.readoutMode()); break;
    }
    printf("\n");
    printf("\tbinning_f: %d\n", rayonixConfig.binning_f());
    printf("\tbinning_s: %d\n", rayonixConfig.binning_s());
    printf("\ttestPattern: %d\n", rayonixConfig.testPattern());
    printf("\texposure: %d ms\n", rayonixConfig.exposure());
    printf("\ttrigger: 0x%08x\n", rayonixConfig.trigger());
    printf("\trawMode: %d\n", rayonixConfig.rawMode());
    printf("\tdarkFlag: %d\n", rayonixConfig.darkFlag());
  }
  void process(const DetInfo &, const Rayonix::ConfigV1 &rayonixConfig)
  {
    char lilbuf[Rayonix::ConfigV1::DeviceIDMax+20];
    printf("*** Processing Rayonix ConfigV1 object\n");
    snprintf(lilbuf, sizeof(lilbuf), "deviceID: '%s'", rayonixConfig.deviceID());
    printf("\t%s\n\treadoutMode: ", lilbuf);
    switch (rayonixConfig.readoutMode()) {
      case Rayonix::ConfigV1::Standard: printf("Standard"); break;
      case Rayonix::ConfigV1::HighGain: printf("HighGain"); break;
      case Rayonix::ConfigV1::LowNoise: printf("LowNoise"); break;
      case Rayonix::ConfigV1::EDR: printf("EDR"); break;
      default: printf("Unrecognized (%d)", (int)rayonixConfig.readoutMode()); break;
    }
    printf("\n");
    printf("\tbinning_f: %d\n", rayonixConfig.binning_f());
    printf("\tbinning_s: %d\n", rayonixConfig.binning_s());
    printf("\texposure: %d ms\n", rayonixConfig.exposure());
    printf("\ttrigger: 0x%08x\n", rayonixConfig.trigger());
    printf("\trawMode: %d\n", rayonixConfig.rawMode());
    printf("\tdarkFlag: %d\n", rayonixConfig.darkFlag());
  }
  void process(const DetInfo &, const SmlData::ConfigV1 &)
  {
    printf("*** Processing SmlData::ConfigV1 object\n");
  }  
  void process(const DetInfo &, const SmlData::ProxyV1 &)
  {
    printf("*** Processing SmlData::ProxyV1 object\n");
  }  
  void process(const DetInfo &, const Partition::ConfigV1 &)
  {
    printf("*** Processing Partition::ConfigV1 object\n");
  }  
  int process(Xtc* xtc) {
    unsigned      i         =_depth; while (i--) printf("  ");
    Level::Type   level     = xtc->src.level();
    printf("%s level  offset %Ld (0x%Lx), payload size %d contains %s damage 0x%x: ",
           Level::name(level), _lliOffset, _lliOffset, xtc->sizeofPayload(), TypeId::name(xtc->contains.id()),
           xtc->damage.value());
    long long lliOffsetPayload = _lliOffset + sizeof(Xtc);
    _lliOffset += sizeof(Xtc) + xtc->sizeofPayload();
     
    const DetInfo& info = *(DetInfo*)(&xtc->src);
    if (level==Level::Source) {
      map<Src,string>::iterator it = aliasMap.find(xtc->src);
      if (it != aliasMap.end()) {
        printf("%s (%s,%d  %s,%d)\n", it->second.c_str(),
               DetInfo::name(info.detector()),info.detId(),
               DetInfo::name(info.device()),info.devId());
      } else {
        printf("%s,%d  %s,%d\n",
               DetInfo::name(info.detector()),info.detId(),
               DetInfo::name(info.device()),info.devId());
      }
    } else {
      ProcInfo& info = *(ProcInfo*)(&xtc->src);
      printf("IpAddress 0x%x ProcessId 0x%x\n",info.ipAddr(),info.processId());
    }
    if (level < 0 || level >= Level::NumberOfLevels )
    {
        printf("Unsupported Level %d\n", (int) level);
        return Continue;
    }    
    switch (xtc->contains.id()) {
    case (TypeId::Id_Xtc) : {
      myLevelIter iter(xtc,_depth+1, lliOffsetPayload);
      iter.iterate();
      break;
    }
    case (TypeId::Id_Frame) :
      process(info, *(const Camera::FrameV1*)(xtc->payload()));
      break;
    case (TypeId::Id_AcqWaveform) :
      process(info, *(const Acqiris::DataDescV1*)(xtc->payload()));
      break;
    case (TypeId::Id_AcqConfig) :
    {      
      unsigned version = xtc->contains.version();
      switch (version) {
      case 1:
        process(info,*(const Acqiris::ConfigV1*)(xtc->payload()));
        break;
      default:
        printf("Unsupported acqiris configuration version %d\n",version);
        break;
      }
      break;      
    }
    case (TypeId::Id_AcqTdcConfig) :
      process(info, *(const Acqiris::TdcConfigV1*)(xtc->payload()));
      break;
    case (TypeId::Id_AcqTdcData) :
      // TdcDataV1 need extra info (XTC size) to get the number of items in it
      process(info, *(const Acqiris::TdcDataV1*)(xtc->payload()), xtc->sizeofPayload());
      break;
    case (TypeId::Id_IpimbData) :
      {
	unsigned version = xtc->contains.version();
	switch (version) {
	case 1:
	  process(info, *(const Ipimb::DataV1*)(xtc->payload()));
	  break;
	case 2:
	  process(info, *(const Ipimb::DataV2*)(xtc->payload()));
	  break;
	default:
	  printf("Unsupported ipimb configuration version %d\n",version);
	  break;
	}
      }
      break;
    case (TypeId::Id_IpimbConfig) :
    {      
      unsigned version = xtc->contains.version();
      switch (version) {
      case 1:
        process(info,*(const Ipimb::ConfigV1*)(xtc->payload()));
        break;
      case 2:
        process(info,*(const Ipimb::ConfigV2*)(xtc->payload()));
        break;
      default:
        printf("Unsupported ipimb configuration version %d\n",version);
        break;
      }
      break;      
    }
    case (TypeId::Id_EncoderData) :
    {      
      unsigned version = xtc->contains.version();
      switch (version) {
      case 1:
        process(info,*(const Encoder::DataV1*)(xtc->payload()));
        break;
      case 2:
        process(info,*(const Encoder::DataV2*)(xtc->payload()));
        break;
      default:
        printf("Unsupported encoder data version %d\n",version);
        break;
      }
      break;      
    }
    case (TypeId::Id_EncoderConfig) :
    {      
      unsigned version = xtc->contains.version();
      switch (version) {
      case 1:
        process(info,*(const Encoder::ConfigV1*)(xtc->payload()));
        break;
      default:
        printf("Unsupported encoder configuration version %d\n",version);
        break;
      }
      break;      
    }
    case (TypeId::Id_TwoDGaussian) :
      process(info, *(const Camera::TwoDGaussianV1*)(xtc->payload()));
      break;
    case (TypeId::Id_Opal1kConfig) :
      process(info, *(const Opal1k::ConfigV1*)(xtc->payload()));
      break;
    case (TypeId::Id_FrameFexConfig) :
      process(info, *(const Camera::FrameFexConfigV1*)(xtc->payload()));
      break;
    case (TypeId::Id_pnCCDconfig) :
      {
      unsigned version = xtc->contains.version();
      switch (version) {
        case 1:
          process(info, *(const PNCCD::ConfigV1*)(xtc->payload()));
          break;
        case 2:
          process(info, *(const PNCCD::ConfigV2*)(xtc->payload()));
          break;
        default:
          printf("Unsupported pnCCD configuration version %d\n",version);
      }
      break;
      }
    case (TypeId::Id_pnCCDframe) :
      {
      process(info, (const PNCCD::FrameV1*)(xtc->payload()));
      break;
      }
    case (TypeId::Id_EvrIOConfig) :
      {      
      process(info, *(const EvrData::IOConfigV1*)(xtc->payload()));
      break;
      }
    case (TypeId::Id_EvrConfig) :
    {      
      unsigned version = xtc->contains.version();
      switch (version) {
      case 1:
        process(info, *(const EvrData::ConfigV1*)(xtc->payload()));
        break;
      case 2:
        process(info, *(const EvrData::ConfigV2*)(xtc->payload()));
        break;
      case 3:
        process(info, *(const EvrData::ConfigV3*)(xtc->payload()));
        break;
      case 4:
        process(info, *(const EvrData::ConfigV4*)(xtc->payload()));
        break;
      case 5:
        process(info, *(const EvrData::ConfigV5*)(xtc->payload()));
        break;
      case 6:
        process(info, *(const EvrData::ConfigV6*)(xtc->payload()));
        break;
      case 7:
        process(info, *(const EvrData::ConfigV7*)(xtc->payload()));
        break;
      default:
        printf("Unsupported evr configuration version %d\n",version);
        break;
      }
      break;      
    }
    case (TypeId::Id_EvrData) :
    {
      process(info, *(const EvrData::DataV3*) xtc->payload() );
      break;        
    }
    case (TypeId::Id_ControlConfig) :
      switch(xtc->contains.version()) {
      case 1:
	process(info, *(const ControlData::ConfigV1*)(xtc->payload()));
	break;
      case 2:
	process(info, *(const ControlData::ConfigV2*)(xtc->payload()));
	break;
      case 3:
	process(info, *(const ControlData::ConfigV3*)(xtc->payload()));
	break;
      default:
	printf("Unsupported ControlData::Config version %d\n",xtc->contains.version());
	break;
      }
      break;
    case (TypeId::Id_Epics) :      
    {
//       int iVersion = xtc->contains.version();
//       if ( iVersion != EpicsXtcSettings::iXtcVersion ) 
//       {
//           printf( "Xtc Epics version (%d) is not compatible with reader supported version (%d)", iVersion, EpicsXtcSettings::iXtcVersion );
//           break;
//       }
      process(info, *(const Epics::EpicsPvHeader*)(xtc->payload()));
      break;
    }
    case (TypeId::Id_TimepixConfig) :
      {
      unsigned version = xtc->contains.version();
      switch (version) {
        case 1:
          process(info, *(const Timepix::ConfigV1*)(xtc->payload()));
          break;
        case 2:
          process(info, *(const Timepix::ConfigV2*)(xtc->payload()));
          break;
        case 3:
          process(info, *(const Timepix::ConfigV3*)(xtc->payload()));
          break;
        default:
          printf("Unsupported timepix configuration version %u\n", version);
          break;
      }
      break;
      }
    case (TypeId::Id_TimepixData) :
      {
      unsigned version = xtc->contains.version();
      switch (version) {
        case 1:
          process(info, *(const Timepix::DataV1*)(xtc->payload()));
          break;
        case 2:
          process(info, *(const Timepix::DataV2*)(xtc->payload()));
          break;
        default:
          printf("Unsupported timepix data version %u\n", version);
          break;
      }
      break;
      }
    /*
     * BLD data
     */
    case (TypeId::Id_FEEGasDetEnergy) :
    {
      switch(xtc->contains.version()) {
      case 0:
        process(info, *(const Bld::BldDataFEEGasDetEnergy*) xtc->payload() );
        break; 
      case 1:
        process(info, *(const Bld::BldDataFEEGasDetEnergyV1*) xtc->payload() );
        break; 
      default:
        break;
      }
    }
    case (TypeId::Id_EBeam) :
    {
      switch(xtc->contains.version()) {
      case 0:
        process(info, *(const Bld::BldDataEBeamV0*) xtc->payload() );
        break; 
      case 1:
        process(info, *(const Bld::BldDataEBeamV1*) xtc->payload() );
        break; 
      case 2:
        process(info, *(const Bld::BldDataEBeamV2*) xtc->payload() );
        break; 
      case 3:
        process(info, *(const Bld::BldDataEBeamV3*) xtc->payload() );
        break; 
      case 4:
        process(info, *(const Bld::BldDataEBeamV4*) xtc->payload() );
        break; 
      case 5:
        process(info, *(const Bld::BldDataEBeamV5*) xtc->payload() );
        break; 
      case 6:
        process(info, *(const Bld::BldDataEBeamV6*) xtc->payload() );
        break; 
      default:
        break;
      }       
      break;
    }    
    case (TypeId::Id_PhaseCavity) :
    {
      process(info, *(const Bld::BldDataPhaseCavity*) xtc->payload() );
      break;        
    }
    case (TypeId::Id_GMD) :
    {
      switch(xtc->contains.version()) {
        case 0:
          process(info, *(const Bld::BldDataGMDV0*) xtc->payload() );
          break;
        case 1:
          process(info, *(const Bld::BldDataGMDV1*) xtc->payload() );
          break;
        default:
          break;
      }
      break;        
    }
    case (TypeId::Id_SharedIpimb) :
    {
     switch(xtc->contains.version()) {
      case 0:
        process(info, *(const Bld::BldDataIpimbV0*) xtc->payload() );
        break; 
      case 1:
        process(info, *(const Bld::BldDataIpimbV1*) xtc->payload() );
        break; 
      default:
        break;
      }       
      break;       
    } 
    case (TypeId::Id_PrincetonConfig) :
    {
      process(info, *(const Princeton::ConfigV1*)(xtc->payload()));
      break;
    }
    case (TypeId::Id_PrincetonFrame) :
    {
      process(info, *(const Princeton::FrameV1*)(xtc->payload()));
      break;
    }    
    case (TypeId::Id_PrincetonInfo) :
    {
      process(info, *(const Princeton::InfoV1*)(xtc->payload()));
      break;
    }    
    case (TypeId::Id_Cspad2x2Element) :
    {
      process(info, *(const CsPad2x2::ElementV1*)(xtc->payload()));
      break;
    }    
    case (TypeId::Id_CspadElement) :
    {
      process(info, *(const CsPad::ElementV1*)(xtc->payload()));
      break;
    }    
    case (TypeId::Id_CspadConfig) :
    {
      process(info, *(const CsPad::ConfigV1*)(xtc->payload()));
      break;
    }    
    case (TypeId::Id_IpmFexConfig) :
    {
      switch(xtc->contains.version()) {
      case 1:
        process(info, *(const Lusi::IpmFexConfigV1*)(xtc->payload()));
        break;
      case 2:
        process(info, *(const Lusi::IpmFexConfigV2*)(xtc->payload()));
        break;
      default:
        printf("Unsupported IpmFexConfig version %d\n",xtc->contains.version());
        break;
      }
    }    
    case (TypeId::Id_IpmFex) :
    {
      process(info, *(const Lusi::IpmFexV1*)(xtc->payload()));
      break;
    }    
    case (TypeId::Id_DiodeFexConfig) :
    {
      switch(xtc->contains.version()) {
      case 1:
        process(info, *(const Lusi::DiodeFexConfigV1*)(xtc->payload()));
        break;
      case 2:
        process(info, *(const Lusi::DiodeFexConfigV2*)(xtc->payload()));
        break;
      default:
        printf("Unsupported DiodeFexConfig version %d\n",xtc->contains.version());
        break;
      }
    }    
    case (TypeId::Id_DiodeFex) :
    {
      process(info, *(const Lusi::DiodeFexV1*)(xtc->payload()));
      break;
    }    
    case (TypeId::Id_TM6740Config):
    {
      switch (xtc->contains.version())
      {
      case 1:
        process(info, *(const Pulnix::TM6740ConfigV1 *) xtc->payload());
        break;
      case 2:
        process(info, *(const Pulnix::TM6740ConfigV2 *) xtc->payload());
        break;
      default:
        printf("Unsupported TM6740Config version %d\n", xtc->contains.version());            
        break;
      }        
      break;
    }
    case (TypeId::Id_PimImageConfig):
    {
      process(info, *(const Lusi::PimImageConfigV1 *) (xtc->payload()));
      break;
    }          
    case (TypeId::Id_AliasConfig):
    {
      unsigned version = xtc->contains.version();
      switch (version) {
      case 1:
        process(info, *(const Alias::ConfigV1 *) (xtc->payload()));
        break;
      default:
        printf("Unsupported alias configuration version %d\n", version);
        break;
      }
      break;
    }          
    case (TypeId::Id_RayonixConfig):
    {
      unsigned version = xtc->contains.version();
      switch (version) {
      case 1:
        process(info, *(const Rayonix::ConfigV1*)(xtc->payload()));
        break;
      case 2:
        process(info, *(const Rayonix::ConfigV2*)(xtc->payload()));
        break;
      default:
        printf("Unsupported Rayonix configuration version %d\n", version);
        break;
      }
      break;
    }
    case (TypeId::Id_SmlDataConfig):
    {
      unsigned version = xtc->contains.version();
      switch (version) {
      case 1:
        process(info, *(const SmlData::ConfigV1*)(xtc->payload()));
        break;
      default:
        printf("Unsupported SmlData Config version %d\n", version);
        break;
      }
      break;
    }
    case (TypeId::Id_SmlDataProxy):
    {
      unsigned version = xtc->contains.version();
      switch (version) {
      case 1:
        process(info, *(const SmlData::ProxyV1*)(xtc->payload()));
        break;
      default:
        printf("Unsupported SmlData Proxy version %d\n", version);
        break;
      }
      break;
    }
    case (TypeId::Id_PartitionConfig):
    {
      unsigned version = xtc->contains.version();
      switch (version) {
      case 1:
        process(info, *(const Partition::ConfigV1*)(xtc->payload()));
        break;
      default:
        printf("Unsupported Partition Config  version %d\n", version);
        break;
      }
      break;
    }
    default :
      printf("Unsupported TypeId %s (value = %d)\n", TypeId::name(xtc->contains.id()), (int) xtc->contains.id());
      break;
    }
    return Continue;
  }
private:
  unsigned       _depth;
  long long int  _lliOffset;

  /* static private data */
  static PNCCD::ConfigV1 _pnCcdCfgListV1[2];
  static PNCCD::ConfigV2 _pnCcdCfgListV2[2];
};

PNCCD::ConfigV1 myLevelIter::_pnCcdCfgListV1[2] = { PNCCD::ConfigV1(), PNCCD::ConfigV1() };
PNCCD::ConfigV2 myLevelIter::_pnCcdCfgListV2[2] = { PNCCD::ConfigV2(), PNCCD::ConfigV2() };

void usage(char* progname) {
  fprintf(stderr,"Usage: %s -f <filename> [-h]\n", progname);
}

int main(int argc, char* argv[]) {
  int c;
  char* xtcname=0;
  int parseErr = 0;

  while ((c = getopt(argc, argv, "hf:")) != -1) {
    switch (c) {
    case 'h':
      usage(argv[0]);
      exit(0);
    case 'f':
      xtcname = optarg;
      break;
    default:
      parseErr++;
    }
  }
  
  if (!xtcname) {
    usage(argv[0]);
    exit(2);
  }

  int fd = open(xtcname, O_RDONLY | O_LARGEFILE);
  if (fd < 0) {
    fprintf(stderr, "Unable to open file '%s'\n", xtcname);
    exit(2);
  }

  XtcFileIterator iter(fd,0x4000000);
  Dgram* dg;
  long long int lliOffset = lseek64( fd, 0, SEEK_CUR );  
  while ((dg = iter.next())) {
    printf("%s transition: time %d.%09d, fid/ticks 0x%0x/0x%x, env 0x%x, offset %Ld (0x%Lx), payloadSize %d\n",
           TransitionId::name(dg->seq.service()),
           dg->seq.clock().seconds(),dg->seq.clock().nanoseconds(),
           dg->seq.stamp().fiducials(),dg->seq.stamp().ticks(), 
           dg->env.value(),
           lliOffset, lliOffset, dg->xtc.sizeofPayload());
    myLevelIter iter(&(dg->xtc),0, lliOffset + sizeof(Xtc) + sizeof(*dg) - sizeof(dg->xtc));
    iter.iterate();
    lliOffset = lseek64( fd, 0, SEEK_CUR ); // get the file offset for the next iteration
  }

  ::close(fd);
  return 0;
}
