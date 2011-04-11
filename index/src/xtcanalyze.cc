#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include <string>

#include "pdsdata/xtc/DetInfo.hh"
#include "pdsdata/xtc/ProcInfo.hh"
#include "pdsdata/xtc/BldInfo.hh"
#include "pdsdata/xtc/XtcIterator.hh"
#include "pdsdata/xtc/XtcFileIterator.hh"
#include "pdsdata/index/IndexFileReader.hh"

using std::string;
using namespace Pds;

class XtcIterWithOffset: public XtcIterator
{
public:
  enum
  { Stop, Continue };
  
  XtcIterWithOffset(Xtc * xtc, unsigned depth, long long int lliOffset) :
    XtcIterator(xtc), _depth(depth), _lliOffset(lliOffset), _iNumXtc(0)
  {}

  int  process(Xtc * xtc);
  
protected:
  unsigned            _depth;
  long long int       _lliOffset;
  int                 _iNumXtc;
};

class XtcIterConfig: public XtcIterWithOffset
{
public:  
  XtcIterConfig(Xtc * xtc, unsigned depth, long long int lliOffset) :
    XtcIterWithOffset(xtc, depth, lliOffset)
  {}

  int  process(Xtc * xtc);  
};

class XtcIterL1Accept: public XtcIterWithOffset
{
public:
  XtcIterL1Accept(Xtc * xtc, unsigned depth, long long int lliOffset):
    XtcIterWithOffset(xtc, depth, lliOffset)
  {}

  int  process(Xtc * xtc);
};

void usage(char *progname)
{
  printf( 
    "Usage:  %s  [-f <xtc filename>] [-i <index>] [-b <begin L1 event#>] "
    "[-n <output L1 event#>] [-c <begin calib cycle#>] [-h]\n"
    "  Options:\n"
    "    -h                       Show usage.\n"
    "    -f <xtc filename>        Set xtc filename\n"
    "    -i <index filename>      Set index filename\n"
    "    -b <begin L1 event#>     Set begin L1 event#\n"
    "    -n <output L1 event#>    Set L1 event# for ouput\n"
    "    -c <begin calib cycle#>  Set begin calib cycle#\n",
    progname
  );
}

int xtcAnalyze( const char* sXtcFilename, const char* sIndexFilename, int iBeginL1Event, int iNumL1Event, int iBeginCalib );

int main(int argc, char *argv[])
{
  int c;
  char* sXtcFilename    = NULL;
  char* sIndexFilename  = NULL;
  int   iBeginL1Event   = 0;
  int   iNumL1Event     = 0;
  int   iBeginCalib     = 0;

  while ((c = getopt(argc, argv, "hf:i:b:n:c:")) != -1)
  {
    switch (c)
    {
    case 'h':
      usage(argv[0]);
      exit(0);
    case 'f':
      sXtcFilename  = optarg;
      break;
    case 'i':
      sIndexFilename= optarg;
      break;
    case 'b':
      iBeginL1Event   = strtol(optarg, NULL, 0);
      break;
    case 'n':
      iNumL1Event     = strtol(optarg, NULL, 0);
      break;
    case 'c':
      iBeginCalib   = strtol(optarg, NULL, 0);
      break;
    default:
      printf( "Unknown option: -%c", c );
    }
  }

  if (!sXtcFilename)
  {
    usage(argv[0]);
    exit(2);
  }

  if ( iNumL1Event < 0 )
  {
    printf( "main(): Output L1 Event # %d < 0\n", iNumL1Event );
    return 0;
  }
    
  return xtcAnalyze( sXtcFilename, sIndexFilename, iBeginL1Event, iNumL1Event, iBeginCalib );
}

int printIndexSummary(const Index::IndexFileReader& indexFileReader)
{
  int             iNumDetector;
  const ProcInfo* lDetector;
  indexFileReader.detectorList(iNumDetector, lDetector);
  printf( "Num of Detector: %d\n", iNumDetector );  
  for ( int iDetector = 0; iDetector < iNumDetector; iDetector++ )
  {
    const ProcInfo& info      = lDetector[iDetector];
    printf( "Segment %d ip 0x%x pid 0x%x\n", 
      iDetector, info.ipAddr(), info.processId());
  }        
  
  int                     iNumCalib;
  const Index::CalibNode* lCalib;
  indexFileReader.calibCyleList(iNumCalib, lCalib);
  printf( "Num of Calib Cycle: %d\n", iNumCalib );  
  for ( int iCalib = 0; iCalib < iNumCalib; iCalib++ )
  {
    const Index::CalibNode& calibNode = lCalib[iCalib];
    printf( "Calib %d Off 0x%Lx L1 %d\n", iCalib, calibNode.lliOffset, calibNode.iL1Index );
  }          
  
  return 0;
}

int printEvent(Index::IndexFileReader& indexFileReader, int iEvent)
{
  int uNumL1Event = 0;
  int iError = indexFileReader.numL1Event(uNumL1Event);
  if ( iError != 0 ) return 1;

  if (iEvent < 0 || iEvent >= (int) uNumL1Event)
  {
    printf("printEvent(): Invalid event %d\n", iEvent);
    return 2;
  }
  
  uint32_t  uFiducial;
  bool      bLnkNext, bLnkPrev;  
  indexFileReader.fiducial(iEvent, uFiducial, bLnkNext, bLnkPrev);
    
  bool bEpics;
  indexFileReader.checkEpics(iEvent, bEpics);
    
  bool bPrinceton;
  indexFileReader.checkPrinceton(iEvent, bPrinceton); 
  
  Damage damage(0);
  indexFileReader.damageSummary(iEvent, damage);
  
  uint32_t uDetDmgMask = 0;
  indexFileReader.detDmgMask(iEvent, uDetDmgMask);
  
  unsigned int uNumEvent = 0;
  const uint8_t* lEvrEvent;
  indexFileReader.evrEventList(iEvent, uNumEvent, lEvrEvent);
  
  unsigned int uNumDamage = 0;
  const Index::SegDmg* lDamage;
  indexFileReader.damageList(iEvent, uNumDamage, lDamage);
  
  printf( "[%d] Fid 0x%05x Dmg 0x%x ", iEvent, uFiducial, damage.value() );
  printf( "Epc %c Prn %c ", (bEpics?'Y':'n'),   (bPrinceton?'Y':'n') );

  /*
   * print events     
   */
  printf( "Evn%d ", uNumEvent );
  if ( uNumEvent != 0 )
    for ( unsigned int uEvent = 0; uEvent < uNumEvent; uEvent++ )
      printf( "[%d] ", (int) lEvrEvent[uEvent] );
  
  /*
   * print segment damages
   */
  printf( "Dmg%d ", uNumDamage );
  if (uNumDamage != 0)
    for ( unsigned int uDmg = 0; uDmg < uNumDamage; uDmg++ )
      printf( "[%d]0x%x ", lDamage[uDmg].index, lDamage[uDmg].damage.value() );
      
  const char* sLink = ( bLnkPrev? (bLnkNext?"<->":"<-o") : (bLnkNext?"o->":"") );
  if ( bLnkPrev||bLnkNext )
    printf( "Lnk %s\n", sLink );
  else
    printf( "\n" );
  
  return 0;  
}

int gotoEvent(const char* sIndexFilename, int iBeginL1Event, int iBeginCalib, int fdXtc, int& iEventAfterSeek)
{
  iEventAfterSeek = -1;
    
  Index::IndexFileReader indexFileReader;
  int iError = indexFileReader.open( sIndexFilename );
  if ( iError != 0 )
    return 1;
  
  int uNumL1Event = 0;
  iError          = indexFileReader.numL1Event(uNumL1Event);
  if ( iError != 0 ) return 2;
    
  printf("Using index file %s to jump to calib %d event %d (Total event %d)\n", 
    sIndexFilename, iBeginCalib, iBeginL1Event, uNumL1Event );
    
  printIndexSummary(indexFileReader);
  
  int iEventAfterCalib = 0;    
  if (iBeginCalib != 0)
  {
    iError = indexFileReader.gotoCalibCycle( iBeginCalib, fdXtc, iEventAfterCalib );
    if ( iError != 0 )
      return 3;   
      
    iBeginL1Event += iEventAfterCalib;
  }

  if (iBeginL1Event < 0 || iBeginL1Event >= (int) uNumL1Event)
  {
    printf("gotoEvent(): Invalid event %d\n", iBeginL1Event);
    return 4;
  }
  
  printEvent(indexFileReader, iBeginL1Event);
    
  if ( iBeginL1Event != iEventAfterCalib )
  {
    iError = indexFileReader.gotoEvent( iBeginL1Event, fdXtc );
    if ( iError != 0 )
      return 5;
  } 
  
  iEventAfterSeek = iBeginL1Event;
  return 0;
}

int genIndexFromXtcFilename( const string& strXtcFilename, string& strIndexFilename )
{  
  unsigned int iFindPos = strXtcFilename.rfind(".xtc");
  
  if (iFindPos == string::npos )
    return 1;
    
  strIndexFilename = strXtcFilename.substr(0, iFindPos) + ".idx";  
  
  struct ::stat statFile;
  int iError = ::stat( strIndexFilename.c_str(), &statFile );
  if ( iError != 0 )
  {
    strIndexFilename.clear();
    return 2;
  }
  
  printf( "Using %s as the index file for analyzing %s\n", 
    strIndexFilename.c_str(), strXtcFilename.c_str() );
  
  return 0;  
}

int xtcAnalyze( const char* sXtcFilename, const char* sIndexFilename, int iBeginL1Event, int iNumL1Event, int iBeginCalib )
{      
  int fdXtc = open(sXtcFilename, O_RDONLY | O_LARGEFILE);
  if (fdXtc < 0)
  {
    printf("Unable to open xtc file %s\n", sXtcFilename);
    return 1;
  }

  int iL1Event    = 0;  
  int iEndL1Event = -1;
  if ( iBeginL1Event != 0 || iBeginCalib != 0 )
  {
    string strIndexFilename;
    
    if ( sIndexFilename == NULL )
    {            
      genIndexFromXtcFilename( sXtcFilename, strIndexFilename );
      if ( strIndexFilename.size() == 0 )
      {
        printf( "xtcAnalyze(): Cannot do fast seeking without using index file" );      
        return 1;
      }
      sIndexFilename = strIndexFilename.c_str();
    }
          
    int iEventAfterSeek = -1;
    int iError = gotoEvent(sIndexFilename, iBeginL1Event, iBeginCalib, fdXtc, iEventAfterSeek);
    if ( iError == 0 )
      iL1Event = iEventAfterSeek;
  }
  
  if ( iNumL1Event > 0 )
    iEndL1Event = iL1Event + iNumL1Event;
  
  XtcFileIterator   iterFile(fdXtc, 0x2000000); // largest L1 data: 32 MB
    
  Dgram *dg;
  long long int lliOffset = lseek64(fdXtc, 0, SEEK_CUR);
  while ((dg = iterFile.next()))
  {             
    bool bEndLoop = false;
    
    switch ( dg->seq.service() )
    {
    case TransitionId::Configure:
    {
      printf( "\n# %s ctl 0x%x vec 0x%x fid 0x%x tick 0x%x "
       "offset 0x%Lx env 0x%x damage 0x%x extent 0x%x\n",
       TransitionId::name(dg->seq.service()), dg->seq.stamp().control(),
       dg->seq.stamp().vector(), dg->seq.stamp().fiducials(), dg->seq.stamp().ticks(), 
       lliOffset, dg->env.value(), dg->xtc.damage.value(), dg->xtc.extent);    
      
      // Go through the config data and create the cfgSegList object
      XtcIterConfig iterConfig(&(dg->xtc), 0, lliOffset + sizeof(*dg) );
      iterConfig.iterate();
      
      break;        
    }
    case TransitionId::L1Accept:
    {
      printf( "\n# %s #%d ctl 0x%x vec 0x%x fid 0x%x tick 0x%x "
       "offset 0x%Lx env 0x%x damage 0x%x extent 0x%x\n",
       TransitionId::name(dg->seq.service()), iL1Event, dg->seq.stamp().control(),
       dg->seq.stamp().vector(), dg->seq.stamp().fiducials(), dg->seq.stamp().ticks(), 
       lliOffset, dg->env.value(), dg->xtc.damage.value(), dg->xtc.extent);    
      
      XtcIterL1Accept iterL1Accept(&(dg->xtc), 0, lliOffset + sizeof(*dg) );
      iterL1Accept.iterate();    
      iL1Event++;
      
      if ( iEndL1Event >= 0 && iL1Event >= iEndL1Event )
        bEndLoop = true;
      break;        
    }
    default:
      printf( "\n# %s ctl 0x%x vec 0x%x fid 0x%x tick 0x%x "
       "offset 0x%Lx env 0x%x damage 0x%x extent 0x%x\n",
       TransitionId::name(dg->seq.service()), dg->seq.stamp().control(),
       dg->seq.stamp().vector(), dg->seq.stamp().fiducials(), dg->seq.stamp().ticks(), 
       lliOffset, dg->env.value(), dg->xtc.damage.value(), dg->xtc.extent);           
       
      XtcIterWithOffset iterDefault(&(dg->xtc), 0, lliOffset + sizeof(*dg) );
      iterDefault.iterate();                   
      break;
    } // switch ( dg->seq.service() )
    
    if ( bEndLoop ) break;
      
    lliOffset = lseek64(fdXtc, 0, SEEK_CUR); // get the file offset for the next iteration
  }

  ::close(fdXtc);
  return 0;
}

int XtcIterWithOffset::process(Xtc * xtc)
{
  Level::Type     level             = xtc->src.level();
  long long       lliOffsetPayload  = _lliOffset + sizeof(Xtc);
  const DetInfo&  info              = (const DetInfo &) (xtc->src);  
  
  if (level == Level::Segment)
  {
    unsigned i = _depth;
    while (i--) printf("  ");
    printf("%s level  offset 0x%Lx damage 0x%x extent 0x%x contains %s V%d ",
     Level::name(level), _lliOffset, 
     xtc->damage.value(), xtc->extent,
     TypeId::name(xtc->contains.id()), xtc->contains.version()
     );
    const ProcInfo & info = (const ProcInfo &) (xtc->src);
    printf("ip 0x%x pid 0x%x\n", info.ipAddr(), info.processId());
    
    if ( _depth != 0 )
      printf( "XtcIterWithOffset::process(): *** Error depth: Expect 0, but get %d\n", _depth );
  }  
  else if (level == Level::Source)
  {
    unsigned i = _depth;
    while (i--) printf("  ");
    printf("%s level  offset 0x%Lx damage 0x%x extent 0x%x contains %s V%d ",
     Level::name(level), _lliOffset, 
     xtc->damage.value(), xtc->extent,
     TypeId::name(xtc->contains.id()), xtc->contains.version()
     );
    printf("src %s,%d %s,%d\n",
     DetInfo::name(info.detector()), info.detId(),
     DetInfo::name(info.device()), info.devId());
     
    if ( _depth != 1 )
      printf( "XtcIterWithOffset::process(): *** Error depth: Expect 1, but get %d\n", _depth );
  }  
  else if (level == Level::Reporter)
  {
    unsigned i = _depth;
    while (i--) printf("  ");
    printf("%s level  offset 0x%Lx damage 0x%x extent 0x%x contains %s V%d ",
     Level::name(level), _lliOffset, 
     xtc->damage.value(), xtc->extent,
     TypeId::name(xtc->contains.id()), xtc->contains.version()
     );     
    const BldInfo & info = *(BldInfo*) (&xtc->src);
    printf("pid 0x%x type %s\n", info.processId(), BldInfo::name(info));
    
    if ( _depth != 1 )
      printf( "XtcIterWithOffset::process(): *** Error depth: Expect 1, but get %d\n", _depth );
  }  
  else
  {    
    unsigned i = _depth;
    while (i--) printf("  ");
    printf("%s level  offset 0x%Lx damage 0x%x extent 0x%x contains %s V%d\n",
     Level::name(level), _lliOffset, 
     xtc->damage.value(), xtc->extent,
     TypeId::name(xtc->contains.id()), xtc->contains.version()
     );
  }  

  _lliOffset += xtc->extent;
  ++_iNumXtc;  
  
  if (xtc->contains.id() == TypeId::Id_Xtc)
  {
    XtcIterWithOffset iter(xtc, _depth + 1, lliOffsetPayload);
    iter.iterate();    
    
    if ( iter._iNumXtc > 5 )
    {
      unsigned i = _depth+1;
      while (i--) printf("  ");
      printf( "Xtc Number %d\n", iter._iNumXtc );    
    }
  }
    
  return XtcIterWithOffset::Continue;     
}

int XtcIterConfig::process(Xtc * xtc)
{
  Level::Type     level             = xtc->src.level();
  long long       lliOffsetPayload  = _lliOffset + sizeof(Xtc);
  const DetInfo&  info              = (const DetInfo &) (xtc->src);  
  
  if (level == Level::Segment)
  {
    unsigned i = _depth;
    while (i--) printf("  ");
    printf("%s level  offset 0x%Lx damage 0x%x extent 0x%x contains %s V%d ",
     Level::name(level), _lliOffset, 
     xtc->damage.value(), xtc->extent,
     TypeId::name(xtc->contains.id()), xtc->contains.version()
     );
    const ProcInfo & info = (const ProcInfo&) (xtc->src);
    printf("ip 0x%x pid 0x%x\n", info.ipAddr(), info.processId());

    if ( _depth != 0 )
      printf( "XtcIterConfig::process(): *** Error depth: Expect 0, but get %d\n", _depth );    
  }  
  else if (level == Level::Source)
  {
    if ( xtc->contains.id() != TypeId::Id_Epics || _iNumXtc < 1 )
    {    
      unsigned i = _depth;
      while (i--) printf("  ");
      printf("%s level  offset 0x%Lx damage 0x%x extent 0x%x contains %s V%d ",
       Level::name(level), _lliOffset, 
       xtc->damage.value(), xtc->extent,
       TypeId::name(xtc->contains.id()), xtc->contains.version()
       );
      printf("src %s,%d %s,%d\n",
       DetInfo::name(info.detector()), info.detId(),
       DetInfo::name(info.device()), info.devId());   
    }
    
    if ( _depth != 1 )
      printf( "XtcIterConfig::process(): *** Error depth: Expect 1, but get %d\n", _depth );
  }
  else if (level == Level::Control)
  {
    unsigned i = _depth;
    while (i--) printf("  ");
    printf("%s level  offset 0x%Lx damage 0x%x extent 0x%x contains %s V%d\n",
     Level::name(level), _lliOffset, 
     xtc->damage.value(), xtc->extent,
     TypeId::name(xtc->contains.id()), xtc->contains.version()
     );
     
    if ( _depth != 1 )
      printf( "XtcIterConfig::process(): *** Error depth: Expect 1, but get %d\n", _depth );
  }
  else if (level == Level::Reporter)
  {
    unsigned i = _depth;
    while (i--) printf("  ");
    printf("%s level  offset 0x%Lx damage 0x%x extent 0x%x contains %s V%d ",
     Level::name(level), _lliOffset, 
     xtc->damage.value(), xtc->extent,
     TypeId::name(xtc->contains.id()), xtc->contains.version()
     );     
    const BldInfo & info = *(BldInfo*) (&xtc->src);
    printf("pid 0x%x type %s\n", info.processId(), BldInfo::name(info));
    
    if ( _depth != 2 )
      printf( "XtcIterL1Accept::process(): *** Error depth: Expect 2, but get %d\n", _depth );
  }    
  else
  {
    unsigned i = _depth;
    while (i--) printf("  ");
    printf("%s level  offset 0x%Lx damage 0x%x extent 0x%x contains %s V%d\n",
     Level::name(level), _lliOffset, 
     xtc->damage.value(), xtc->extent,
     TypeId::name(xtc->contains.id()), xtc->contains.version()
     );
     
    printf( "XtcIterConfig::process(): *** Error level %s depth = %d\n", Level::name(level), _depth );     
  }

  _lliOffset += xtc->extent;
        
  ++_iNumXtc;  
  if (xtc->contains.id() == TypeId::Id_Xtc)
  {
    XtcIterConfig iter(xtc, _depth + 1, lliOffsetPayload);
    iter.iterate();
    
    if ( iter._iNumXtc > 5 )
    {
      unsigned i = _depth+1;
      while (i--) printf("  ");
      printf( "Xtc Number %d\n", iter._iNumXtc );    
    }
  }
    
  return XtcIterConfig::Continue;     
}

int XtcIterL1Accept::process(Xtc * xtc)
{
  Level::Type     level             = xtc->src.level();
  long long       lliOffsetPayload  = _lliOffset + sizeof(Xtc);
  const DetInfo&  info              = (const DetInfo &) (xtc->src);
  
  if (level == Level::Segment)
  {
    unsigned i = _depth;
    while (i--) printf("  ");
    printf("%s level  offset 0x%Lx damage 0x%x extent 0x%x contains %s V%d ",
     Level::name(level), _lliOffset, 
     xtc->damage.value(), xtc->extent,
     TypeId::name(xtc->contains.id()), xtc->contains.version()
     );
    const ProcInfo & info = (const ProcInfo &) (xtc->src);
    printf("ip 0x%x pid 0x%x\n", info.ipAddr(), info.processId());
    
    if ( _depth != 0 )
      printf( "XtcIterL1Accept::process(): *** Error depth: Expect 0, but get %d\n", _depth );
  }  
  else if (level == Level::Source)
  {
    if ( xtc->contains.id() != TypeId::Id_Epics || _iNumXtc < 1 )
    {    
      unsigned i = _depth;
      while (i--) printf("  ");
      printf("%s level  offset 0x%Lx damage 0x%x extent 0x%x contains %s V%d ",
       Level::name(level), _lliOffset, 
       xtc->damage.value(), xtc->extent,
       TypeId::name(xtc->contains.id()), xtc->contains.version()
       );
      printf("src %s,%d %s,%d\n",
       DetInfo::name(info.detector()), info.detId(),
       DetInfo::name(info.device()), info.devId());
    }
  
    if ( _depth != 1 )
      printf( "XtcIterL1Accept::process(): *** Error depth: Expect 1, but get %d\n", _depth );
  }  
  else if (level == Level::Reporter)
  {
    unsigned i = _depth;
    while (i--) printf("  ");
    printf("%s level  offset 0x%Lx damage 0x%x extent 0x%x contains %s V%d ",
     Level::name(level), _lliOffset, 
     xtc->damage.value(), xtc->extent,
     TypeId::name(xtc->contains.id()), xtc->contains.version()
     );     
    const BldInfo & info = *(BldInfo*) (&xtc->src);
    printf("pid 0x%x type %s\n", info.processId(), BldInfo::name(info));
    
    if ( _depth != 1 )
      printf( "XtcIterL1Accept::process(): *** Error depth: Expect 1, but get %d\n", _depth );
  }  
  else
  {    
    unsigned i = _depth;
    while (i--) printf("  ");
    printf("%s level  offset 0x%Lx damage 0x%x extent 0x%x contains %s V%d\n",
     Level::name(level), _lliOffset, 
     xtc->damage.value(), xtc->extent,
     TypeId::name(xtc->contains.id()), xtc->contains.version()
     );
     
    printf( "XtcIterL1Accept::process(): *** Error level %s depth = %d\n", Level::name(level), _depth );
  }  

  _lliOffset += xtc->extent;
      
  ++_iNumXtc;
  if (xtc->contains.id() == TypeId::Id_Xtc)
  {
    XtcIterL1Accept iter(xtc, _depth + 1, lliOffsetPayload);
    iter.iterate();        
    
    if ( iter._iNumXtc > 5 )
    {
      unsigned i = _depth+1;
      while (i--) printf("  ");
      printf( "Xtc Number %d\n", iter._iNumXtc );    
    }    
  }
    
  return XtcIterL1Accept::Continue;     
}

//int gotoEvent2(char* sIndexFilename, int iEvent, int fdXtc)
//{
//  Index::IndexList indexList;
//  readIndex( sIndexFilename, indexList );
//  
//  Index::L1AcceptNode* pNode = NULL;
//  indexList.getNode( iEvent, pNode );
//    
//  long long int lliOffsetSeek = lseek64(fdXtc, pNode->lliOffsetXtc, SEEK_SET);
//  if ( lliOffsetSeek != pNode->lliOffsetXtc )
//  {
//    printf( "gotoEvent(): Failed to jump to %d event at offset 0x%Lx (actual offset 0x%Lx)\n",
//      iEvent, pNode->lliOffsetXtc, lliOffsetSeek );
//    lseek64(fdXtc, 0, SEEK_SET);
//    
//    return 1;
//  }
//  
//  return 0;
//}
//
//int readIndex(char* sInputIndex, Index::IndexList& indexList)
//{
//  int fd = open(sInputIndex, O_RDONLY | O_LARGEFILE);
//  if (fd < 0)
//  {
//    printf("Unable to open xtc file %s\n", sInputIndex);
//    return 1;
//  }
//  
//  indexList.readFromFile(fd);
//  
//  ::close(fd);
//  
//  int iVerbose = 1;
//  indexList.printList(iVerbose);  
//      
//  return 0;
//}
