#include <errno.h>
#include <libgen.h>

#include "pdsdata/evr/DataV3.hh"
#include "pdsdata/xtc/BldInfo.hh"
#include "pdsdata/index/IndexList.hh"

namespace Pds
{  
namespace Index
{
/*
 * class IndexList
 */ 

int IndexList::startNewNode(const Dgram& dg, int64_t lliOffset, bool& bInvalidData)
{
  bInvalidData = false;
  
  int iError = 0;    
  if ( _bNewNode )  
  {
    printf( "IndexList::startNewNode():: *** Previous new node hasn't been finished\n" );
    finishNode(false);    
    iError = 1;
  }

  uint32_t uFiducial = dg.seq.stamp().fiducials();  
  
  if ( uFiducial == L1AcceptNode::uInvalidFiducial )
  {
    bInvalidData = true;
    return 2;
  }
    
  L1AcceptNode  nodeNew(dg.seq.clock().seconds(), dg.seq.clock().nanoseconds(), uFiducial, lliOffset);
  L1AcceptNode& nodeCur = checkInNode(nodeNew);
  
  _pCurNode = &nodeCur;
  
  nodeCur.damage = dg.xtc.damage;
    
  _bNewNode = true;  
  _itCurSeg = _mapSegToId.end();
  
  return iError;
}

int IndexList::updateSegment(const Xtc& xtc)
{
  if ( ! _bNewNode )  
  {
    printf( "IndexList::updateSegment():: *** new node hasn't been initialized\n" );
    return 1;
  }

  finishPrevSegmentId();
  
  const ProcInfo& info          = (const ProcInfo &) (xtc.src);  
  int             iSegmentIndex = -1;  
  
  if ( _iNumSegments < 0 )
  {    
    iSegmentIndex = _mapSegToId.size();
            
    std::pair<TSegmentToIdMap::iterator,bool> 
      rInsert = _mapSegToId.insert( 
        TSegmentToIdMap::value_type( info, L1SegmentId(iSegmentIndex) ) );
      
    if ( !rInsert.second )
    {
      printf( "IndexList::updateSegment():: Found duplicated segment for "
        "ip 0x%x pid 0x%x\n", info.ipAddr(), info.processId() );
      return 2;
    }
    
    _itCurSeg = rInsert.first;
  }
  else
  {
    TSegmentToIdMap::iterator itFind = 
      _mapSegToId.find( info );
      
    if ( itFind == _mapSegToId.end() )
    {
      printf( "IndexList::updateSegment():: *** cannot find segment node for "
        "ip 0x%x pid 0x%x\n", info.ipAddr(), info.processId() );
      return 3;
    }     
  
    iSegmentIndex = itFind->second.iIndex;
    if ( iSegmentIndex >= (int) _iNumSegments )
    {
      printf( "IndexList::updateSegment():: *** invalid segment index %d (max value %d)\n", 
        iSegmentIndex, _iNumSegments );
      return 4;
    }
    _itCurSeg                   = itFind;
  }

  L1AcceptNode& node  =   *_pCurNode;  
  if ( xtc.damage.value() != 0 )
    node.uMaskDetDmgs   |=  (1 << iSegmentIndex);  
  
  if ( xtc.contains.id() != TypeId::Id_Xtc ) // not a normal segment level
    return 5;

  if ( xtc.sizeofPayload() == 0 )
    node.uMaskDetData   |=  (1 << iSegmentIndex);
    
  return 0;
}

int IndexList::updateSource(const Xtc& xtc, bool& bStopUpdate)
{
  bStopUpdate = false;
  
  if ( ! _bNewNode )  
  {
    printf( "IndexList::updateSource():: *** new node hasn't been initialized\n" );
    return 1;
  }
  
  if ( _itCurSeg == _mapSegToId.end() )
  {
    printf( "IndexList::updateSource():: Invalid segment\n" );
    return 2;
  }

  L1SegmentId&  segId = _itCurSeg->second;
  if (segId.bSrcUpdated)
    bStopUpdate = true;
  else
  {
    segId.srcList .push_back(xtc.src);
    segId.typeList.push_back(xtc.contains);
    
    if (xtc.contains.id() == TypeId::Id_Epics)
      bStopUpdate = true;
  }
  
  L1AcceptNode& node  = *_pCurNode;
  if (xtc.contains.id() == TypeId::Id_Epics)
    node.bEpics     = true;  
  else if ( xtc.contains.id() == TypeId::Id_PrincetonFrame )
    node.bPrinceton = true;  
  else if ( xtc.contains.id() == TypeId::Id_EvrData )
    updateEvr( xtc );

  return 0;
}

int IndexList::updateReporter(const Xtc& xtc, bool& bStopUpdate)
{
  bStopUpdate = false;
  
  if ( ! _bNewNode )  
  {
    printf( "IndexList::updateReporter():: *** new node hasn't been initialized\n" );
    return 1;
  }
  
  if ( _itCurSeg == _mapSegToId.end() )
  {
    printf( "IndexList::updateReporter():: Invalid segment\n" );
    return 2;
  }

  L1SegmentId&  segId = _itCurSeg->second;
  if (segId.bSrcUpdated)
    bStopUpdate = true;
  else
  {
    segId.srcList .push_back(xtc.src);
    segId.typeList.push_back(xtc.contains);
  }
  
  return 0;
}

int IndexList::updateEvr(const Xtc& xtc)
{
  // assume xtc.contains.id() == TypeId::Id_EvrData
  
  if ( xtc.contains.version() != 3 )
  {
    printf( "UnExtorted Evr Data Ver %d\n", xtc.contains.version() );
    return 1;
  }
  
  const EvrData::DataV3& evrData = * reinterpret_cast<const EvrData::DataV3*>(xtc.payload());
  
  for ( unsigned int uEvent = 0; uEvent < evrData.numFifoEvents(); uEvent++ )
  {
    const EvrData::DataV3::FIFOEvent& fifoEvent = 
      evrData.fifoEvent(uEvent);
      
    TEvrEvtToIdMap::iterator itFind = 
      _mapEvrToId.find(fifoEvent.EventCode);
      
    if ( itFind == _mapEvrToId.end() )
    {
      std::pair<TEvrEvtToIdMap::iterator,bool> 
        rInsert = _mapEvrToId.insert( 
          TEvrEvtToIdMap::value_type( fifoEvent.EventCode, _mapEvrToId.size() ) );
      itFind = rInsert.first;
    }
            
    _pCurNode->uMaskEvrEvents |= ( 1 << itFind->second );
  }  
  
  return 0;
}

int IndexList::finishNode(bool bPrint)
{
  if ( ! _bNewNode )  
  {
    printf( "IndexList::finishNode():: *** new node hasn't been initialized\n" );
    return 1;
  }

  finishPrevSegmentId();
  
  L1AcceptNode& node  =   *_pCurNode;  
  /*
   * Convert segment damage mask from un-ordered bits to ordered bits
   */
  if ( _iNumSegments < 0 )
  {
    _iNumSegments = _mapSegToId.size();
    
    uint32_t  uMaskBit        = 0x1;
    uint32_t  uMaskDamageNew  = 0;
    uint32_t  uMaskDetDataNew = 0;
    
    int       iMapIndex       = 0;
    for ( TSegmentToIdMap::iterator 
      iterMap =  _mapSegToId.begin();
      iterMap != _mapSegToId.end();
      iterMap++, iMapIndex++, uMaskBit<<=1 )
    {
      int iSegIndex = iterMap->second.iIndex;
      //const ProcInfo& info      = iterMap->first.procNode;
      //printf( "Segment %d [org %d] ip 0x%x pid 0x%x\n", 
      //  iMapIndex, iSegIndex, info.ipAddr(), info.processId()); // !! debug
      
      if ( (node.uMaskDetDmgs & (1<<iSegIndex)) != 0 )
        uMaskDamageNew |= uMaskBit;

      if ( (node.uMaskDetData & (1<<iSegIndex)) != 0 )
        uMaskDetDataNew |= uMaskBit;
        
      iterMap->second.iIndex = iMapIndex;
    }      
    
    node.uMaskDetDmgs = uMaskDamageNew;
    node.uMaskDetData = uMaskDetDataNew;
  } // if ( _iNumSegments < 0 )
  
  if (bPrint)
    printNode(*_pCurNode, _iCurSerial);
      
  _bNewNode = false;   
  _pCurNode = NULL;  
  return 0;
}

int IndexList::finishList()
{    
  if ( _mapSegToId.size() > 32 )
    printf( "IndexList::finishList(): segment # %d > 32! It will make invalid index file\n",
      _mapSegToId.size() );
      
  if ( _mapEvrToId.size() > 32 )
    printf( "IndexList::finishList(): evr event # %d > 32! It will make invalid index file\n",
      _mapEvrToId.size() );
  
  /*
   * Calculate file size
   */
  unsigned int uFileSizeOrg = 
    sizeof(IndexFileHeaderType)                       +
    sizeof(CalibNode)           * _lCalib     .size() +
    sizeof(uint8_t)             * _mapEvrToId .size() ;  
    
  for ( TSegmentToIdMap::const_iterator 
    itMap =  _mapSegToId.begin();
    itMap != _mapSegToId.end();
    itMap++ )
  {
    const L1SegmentId&  segId = itMap->second;
    unsigned int        uSize = 
      sizeof(ProcInfo) +
      sizeof(uint8_t)  + 
      segId.srcList.size()  * sizeof(segId.srcList[0]) +
      segId.typeList.size() * sizeof(segId.typeList[0]);    
    uFileSizeOrg += uSize;     
  }
  
  // adjust the size so that the header size is a multiple of 4
  unsigned int uHeaderPadding = 0;
  if (uFileSizeOrg % 4 != 0)
    uHeaderPadding =  (4 - uFileSizeOrg % 4);
    
  uFileSizeOrg += uHeaderPadding; 
  _iHeaderSize =  uFileSizeOrg;    
  
  // Add main content size
  uFileSizeOrg += sizeof(IndexFileL1NodeType) * _lNode.size() ;
      
  _uFileSize   = uFileSizeOrg;  
  
  return 0;
}

int IndexList::addCalibCycle(int64_t lliOffset)
{  
  _lCalib.push_back(CalibNode(lliOffset,_lNode.size()));
  return 0;
}

void IndexList::printList(int iVerbose) const
{ 
  printf( "XtcFn %s List index# 0x%x (%u) Calib# %d EvrEvt# %d Segment# %u HSize 0x%x File size 0x%x (%u)\n",
    _sXtcFilename, _lNode.size(), _lNode.size(), 
    _lCalib.size(), _mapEvrToId.size(), _mapSegToId.size(), 
    _iHeaderSize, _uFileSize, _uFileSize );
  
  if ( iVerbose > 0 )
  {
    /*
     * Print BeginCalibCycle list
     */    
    int iCalib = 0;
    for ( TCalibList::const_iterator
      iterCalib =  _lCalib.begin();
      iterCalib != _lCalib.end();
      iterCalib++, iCalib++ )
    {
      const CalibNode& calibNode = *iterCalib;
      printf( "Calib %d Off 0x%Lx L1 %d\n", iCalib, calibNode.lliOffset, calibNode.iL1Index );
    }
    
    /*
     * Print Event Code list
     */    
    for ( TEvrEvtToIdMap::const_iterator 
      iterMap =  _mapEvrToId.begin();
      iterMap != _mapEvrToId.end();
      iterMap++ )
    {
      printf( "Evr Event %d Code %d\n", 
        iterMap->second, iterMap->first );
    }
    
    /*
     * Print segment list
     */    
    for ( TSegmentToIdMap::const_iterator 
      iterMap =  _mapSegToId.begin();
      iterMap != _mapSegToId.end();
      iterMap++ )
    {
      const ProcInfo& info    = iterMap->first.procNode;
      printf( "Segment %d ip 0x%x pid 0x%x\n", 
        iterMap->second.iIndex, info.ipAddr(), info.processId());
        
      /*
       * Print typeId and src list for each segment
       */        
      const L1SegmentId& segId = iterMap->second;
      for ( int iSource = 0; iSource < (int) segId.srcList.size(); iSource++ )
      {         
        const Src& src = (Src&) segId.srcList[iSource];
        if (src.level() == Level::Source)
        {        
          const DetInfo& info = (const DetInfo&) src;
          printf("  src %s,%d %s,%d ",
           DetInfo::name(info.detector()), info.detId(),
           DetInfo::name(info.device()), info.devId());           
        }
        else if ( src.level() == Level::Reporter )
        {
          const BldInfo& info = (const BldInfo&) src;
          printf("  bldType %s ", BldInfo::name(info));          
        }
          
        const TypeId& typeId = segId.typeList[iSource];
        printf("contains %s V%d\n",
         TypeId::name(typeId.id()), typeId.version()
         );          
      }
    }
  } // if ( iVerbose > 0 )
  
  if ( iVerbose > 1 )
  {
    int iSerial = 0;
    for ( TNodeList::const_iterator 
      iterNode = _lNode.begin(); 
      iterNode != _lNode.end(); 
      ++iterNode, iSerial++ )
    {
      printNode(*iterNode, iSerial);
    }
  }
}

void IndexList::printNode(const L1AcceptNode& node, int iSerial) const
{
  char sTimeBuff[128];
  time_t t = node.uSeconds;
  strftime(sTimeBuff,128,"%T",localtime(&t));  
  
  printf( "[%d] %s.%03u Fid 0x%05x Off 0x%Lx Dmg 0x%x ", 
    iSerial, sTimeBuff, (int)(node.uNanoseconds/1e6), 
    node.uFiducial, node.lliOffsetXtc, node.damage.value() );

  printf( "DetDmg 0x%x DetData 0x%x ", node.uMaskDetDmgs, node.uMaskDetData );
  
  /*
   * print events     
   */
  if ( node.uMaskEvrEvents != 0 )
  {
    printf("Evn ");
    uint32_t uEventBit = 0x1;
    for ( int iBit = 0; iBit < (int) sizeof(node.uMaskEvrEvents)*8; iBit++, uEventBit <<= 1 )
      if ( (node.uMaskEvrEvents & uEventBit) != 0 )
        for ( TEvrEvtToIdMap::const_iterator 
          iterMap =  _mapEvrToId.begin();
          iterMap != _mapEvrToId.end();
          iterMap++ )
        {
          if ( iterMap->second == iBit )
            printf( "[%d] ", iterMap->first );
        }    
  }
  
  printf("\n");
}

int IndexList::writeToFile(int fdFile) const
{  
  int iError1 = writeFileInfoHeader (fdFile);
  int iError2 = writeFileMainContent(fdFile);
  
  int iError = 
    (iError1 != 0 ? 1 : 0) |
    (iError2 != 0 ? 2 : 0) ;
  return iError;
}

int IndexList::finishPrevSegmentId()
{
  if ( _itCurSeg == _mapSegToId.end() )
    return 0;

  L1SegmentId&  segId = _itCurSeg->second;
  
  if (segId.srcList.size() != 0 )
    segId.bSrcUpdated   = true;
  
  return 0;
}

int IndexList::readFromFile(int fdFile)
{  
  int iNumIndex = 0;  
  int iError1 = readFileInfoHeader  (fdFile, iNumIndex);  
  if ( iError1 != 0 )
    return 1;    
  
  int iError = readFileMainContent (fdFile, iNumIndex);
  
  return iError;
}

IndexList::IndexList()
{
  reset();
  memset( _sXtcFilename, 0, sizeof(_sXtcFilename) );
}

IndexList::IndexList(const char* sXtcFilename)
{
  reset();
  setXtcFilename(sXtcFilename);
}

int IndexList::reset()
{
  memset( _sXtcFilename, 0, sizeof(_sXtcFilename) );
  _iNumSegments = -1;
  _bNewNode     = false;
  _pCurNode     = NULL;
  _uFileSize    = 0;
  _iCurSerial   = -1;

  _lCalib     .clear();
  _lNode      .clear();
  _mapSegToId .clear();  
  _mapEvrToId .clear();
  
  return 0;
}

int IndexList::setXtcFilename(const char* sXtcFilename)
{
  char* sFnCopy = strdup(sXtcFilename);
  char* sBaseFn = basename(sFnCopy);
  
  int iError = 0;
  
  if ( (int) strlen( sBaseFn) >= iMaxFilenameLen)
  {
    printf( "IndexList::setXtcFilename(): xtc filename %s is too long (max=%d), "
      "filename will be truncated.\n", 
      sBaseFn, iMaxFilenameLen-1 );
    iError = 1;
  }
  
  strncpy( _sXtcFilename,  sBaseFn, iMaxFilenameLen-1);     
  _sXtcFilename[iMaxFilenameLen-1] = 0;
  free(sFnCopy);  
  
  return iError;
}

int IndexList::getNumNode() const
{
  return _lNode.size();
}

int IndexList::getNode(int iNode, L1AcceptNode*& pNode)
{
  if ( iNode < 0 || iNode >= (int) _lNode.size() )
    return 1;
    
  pNode = &_lNode[iNode];
  return 0;
}

L1AcceptNode& IndexList::checkInNode( L1AcceptNode& nodeNew )
{
  if ( _lNode.size() == 0 )
  {      
    _lNode.push_back( nodeNew );
    _iCurSerial = 0;
    return _lNode.back();
  }
    
  L1AcceptNode& nodeLast = _lNode.back();
    
  if ( nodeNew.laterThan(nodeLast) )
  {
    if ( nodeNew.uFiducial ==  nodeLast.uFiducial )
    {
      printf( "IndexList::checkInNode(): *** event 0x%x has the same fiducial as the last one\n", 
        nodeNew.uFiducial ); // !! debug
          
      nodeLast.bLnkNext = true;
      nodeNew. bLnkPrev = true;
    }
    
    _lNode.push_back( nodeNew );
    _iCurSerial = _lNode.size() - 1;
    return _lNode.back();
  }
  
  int iSerial = _lNode.size() - 2;
  for ( TNodeList::iterator 
    itNodeIns =  _lNode.end() - 2;
    itNodeIns >= _lNode.begin();
    itNodeIns--, iSerial-- )
  {
    if ( nodeNew.laterThan(*itNodeIns) )
    {
      if ( nodeNew.uFiducial == itNodeIns->uFiducial  )
      {
        printf( "IndexList::checkInNode(): *** event 0x%x has the same fiducial as the previous (%d older)\n", 
          nodeNew.uFiducial, _lNode.end() - itNodeIns ); // !! debug
        itNodeIns->bLnkNext = true;
        nodeNew.   bLnkPrev = true;          
      }
      else        
        printf( "IndexList::checkInNode(): *** event 0x%x need to be inserted in older position (%d older, fid 0x%x)\n", 
          nodeNew.uFiducial, _lNode.end() - itNodeIns - 1, itNodeIns->uFiducial); // !! debug
      
      TNodeList::iterator iterNew = _lNode.insert( itNodeIns+1, nodeNew );
      _iCurSerial = iSerial+1;
      return *iterNew;
    }          
  }  

  /*
   * Remaining case:
   *   The new node should be inserted at the front of the list
   */
  printf( "IndexList::checkInNode(): *** event 0x%x need to be inserted at front (total %d elements)\n", 
    nodeNew.uFiducial, _lNode.size() ); // !! debug
   
  _lNode.insert( _lNode.begin(), nodeNew );
  _iCurSerial = 0;
  return _lNode.front();  
}

int IndexList::writeFileInfoHeader(int fdFile) const
{  
  int iError = -1;
    
  /*
   * Write main header
   */
  IndexFileHeaderType fileHeader(*this);
  iError = ::write(fdFile, &fileHeader, sizeof(fileHeader) );
  if ( iError == -1 )
    printf( "IndexList::writeFileInfoHeader(): write file info header failed (%s)\n", strerror(errno) );
    
  /*
   * Write BeginCalibCycle
   */
  iError = ::write(fdFile, &_lCalib[0], _lCalib.size() * sizeof(_lCalib[0]) );  
  if ( iError == -1 )
    printf( "IndexList::writeFileInfoHeader(): write calib node failed (%s)\n", strerror(errno) );    
    
  /*
   * Write event codes
   */
  uint8_t lEventCodes[_mapEvrToId.size()];  
  memset(lEventCodes, 0, sizeof(lEventCodes));
  for ( TEvrEvtToIdMap::const_iterator 
    itMap =  _mapEvrToId.begin();
    itMap != _mapEvrToId.end();
    itMap++ )
  {    
    if ( itMap->second >= (int) _mapEvrToId.size() )
      printf("IndexList::writeFileInfoHeader(): Found invalid evr index %d for code %d\n",
        itMap->second, itMap->first);
    else
      lEventCodes[itMap->second] = itMap->first;
  }
    
  iError = ::write(fdFile, &lEventCodes, sizeof(lEventCodes) );
  if ( iError == -1 )
    printf( "IndexList::writeFileInfoHeader(): write event code failed (%s)\n", strerror(errno) );    

  /*
   * Write segments
   */    
  for ( TSegmentToIdMap::const_iterator 
    itMap =  _mapSegToId.begin();
    itMap != _mapSegToId.end();
    itMap++ )
  {
    const ProcInfo& info  = (itMap->first).procNode;
    iError = ::write(fdFile, &info, sizeof(info) );
    if ( iError == -1 )
      printf( "IndexList::writeFileInfoHeader(): write proc info failed (%s)\n", strerror(errno) );    

    /*
     * Write source and typeId list for each segment
     */    
    const L1SegmentId& segId = itMap->second;
    uint8_t uNumSrc = segId.srcList.size();
    iError = ::write(fdFile, &uNumSrc, sizeof(uNumSrc) );
    if ( iError == -1 )
      printf( "IndexList::writeFileInfoHeader(): write NumSrc failed (%s)\n", strerror(errno) );    
      
    iError = ::write(fdFile, &segId.srcList[0], segId.srcList.size() * sizeof(segId.srcList[0]) );  
    if ( iError == -1 )
      printf( "IndexList::writeFileInfoHeader(): write src list failed (%s)\n", strerror(errno) );    

    iError = ::write(fdFile, &segId.typeList[0], segId.typeList.size() * sizeof(segId.typeList[0]) );  
    if ( iError == -1 )
      printf( "IndexList::writeFileInfoHeader(): write type list failed (%s)\n", strerror(errno) );               
  }    
  
  int64_t lliOffset = lseek64(fdFile, 0, SEEK_CUR);
  unsigned int uHeaderPadding = 0;
  if (lliOffset % 4 != 0)
    uHeaderPadding =  (4 - lliOffset % 4);    
  printf( "Write Header Padding %d bytes\n", uHeaderPadding );
  if ( uHeaderPadding > 0 )
  {    
    uint8_t lcPadding[uHeaderPadding];
    memset( lcPadding, 0, sizeof(lcPadding) );
    int iError = ::write(fdFile, &lcPadding, sizeof(lcPadding) );
    if ( iError == -1 )
      printf( "IndexList::writeFileInfoHeader(): write header padding failed (%s)\n", strerror(errno) );     
  }  
  
  return 0;
}

int IndexList::writeFileMainContent(int fdFile) const 
{
  int iNode = 0;
  
  for (TNodeList::const_iterator 
    iterNode =  _lNode.begin();
    iterNode != _lNode.end();
    iterNode++, iNode++)
  {
    IndexFileL1NodeType node(*iterNode);

    int iError = ::write(fdFile, &node, sizeof(node) );
    if ( iError == -1 )
      printf( "IndexList::writeFileMainContent(): write node %d failed (%s)\n", iNode, strerror(errno) );              
  }

  int64_t lliOffset    = lseek64(fdFile, 0, SEEK_CUR);  
  if ( lliOffset != _uFileSize )
  {
    printf( "IndexList::writeFileMainContent(): file size calculation incorrect, expected 0x%x, actual 0x%Lx\n",
      _uFileSize, lliOffset);
    return 1;     
  }
  
  return 0;
}

int IndexList::readFileInfoHeader(int fdFile, int& iNumIndex)
{
  int iError;

  /*
   * Read main header
   */
  IndexFileHeaderType fileHeader;  
  iError = ::read(fdFile, &fileHeader, sizeof(fileHeader) );
  if ( iError == -1 )
  {
    printf( "IndexList::readFileInfoHeader(): read file info header failed (%s)\n", strerror(errno) );
    return 1;
  }
    
  if ( fileHeader.xtcIndex.contains.id() != TypeId::Id_Index &&
    (int) fileHeader.xtcIndex.contains.version() != IndexFileHeaderType::iXtcIndexVersion )
  {
    printf( "IndexList::readFileInfoHeader(): UnExtorted xtc type: %s V%d\n",
      TypeId::name(fileHeader.xtcIndex.contains.id()),
      fileHeader.xtcIndex.contains.version() );
    return 2;
  }
  
  _uFileSize    = fileHeader.xtcIndex.extent;  
  _iHeaderSize  = fileHeader.iHeaderSize;
  iNumIndex     = fileHeader.iNumIndex;
  strncpy( _sXtcFilename, fileHeader.sXtcFilename, iMaxFilenameLen-1);    
  _sXtcFilename[iMaxFilenameLen-1] = 0;
  
  /*
   * Read BeginCalibCycle
   */
  int iNumCalib = fileHeader.iNumCalib;  
  _lCalib.resize(iNumCalib);
  if (iNumCalib > 0)
  {
    iError = ::read(fdFile, &_lCalib[0], _lCalib.size() * sizeof(_lCalib[0]) );
    if ( iError == -1 )
    {
      printf( "IndexList::readFileInfoHeader(): read calib failed (%s)\n", strerror(errno) );
      return 4;
    }
  }

  /*
   * Read event codes
   */
  int iNumEvrEvents = fileHeader.iNumEvrEvents;
  if ( iNumEvrEvents < 0 )
  {
    printf("IndexList::readFileInfoHeader(): Invalid Evr event # %d\n", iNumEvrEvents);
    iNumEvrEvents = 0;
  }
  else if ( iNumEvrEvents > 0 )
  {
    uint8_t lEventCodes[iNumEvrEvents];  
    iError = ::read(fdFile, lEventCodes, sizeof(lEventCodes) );
    if ( iError == -1 )
    {
      printf( "IndexList::readFileInfoHeader(): read Evr events failed (%s)\n", strerror(errno) );
      return 5;
    }
        
    for (int iEvrEvent = 0; iEvrEvent < iNumEvrEvents; ++iEvrEvent )
      _mapEvrToId.insert( 
        TEvrEvtToIdMap::value_type( (int) lEventCodes[iEvrEvent], iEvrEvent ) );
  }
  
  /*
   * Read segments
   */    
  _iNumSegments = fileHeader.iNumDetector;  
  _mapSegToId.clear();
  for ( int iSeg = 0; iSeg < _iNumSegments; iSeg++ )
  {
    ProcInfo info(Level::Segment,0,0);
    iError = ::read(fdFile, &info, sizeof(info) );
    if ( iError == -1 )
      printf( "IndexList::readFileInfoHeader(): read proc info failed (%s)\n", strerror(errno) );
    
    std::pair<TSegmentToIdMap::iterator,bool> 
      rInsert = _mapSegToId.insert( 
        TSegmentToIdMap::value_type( info, L1SegmentId(iSeg) ) );
      
    if ( !rInsert.second )
    {
      printf( "IndexList::readFileInfoHeader(): read duplicated segment for "
        "ip 0x%x pid 0x%x\n", info.ipAddr(), info.processId() );
    }
    
    uint8_t uNumSrc;
    iError = ::read(fdFile, &uNumSrc, sizeof(uNumSrc) );
    if ( iError == -1 )
      printf( "IndexList::readFileInfoHeader(): read NumSrc failed (%s)\n", strerror(errno) );        
    
    L1SegmentId& segId = rInsert.first->second;
    segId.srcList .resize(uNumSrc);
    segId.typeList.resize(uNumSrc);
    if (uNumSrc > 0)
    {
      iError = ::read(fdFile, &segId.srcList[0], segId.srcList.size() * sizeof(segId.srcList[0]) );
      if ( iError == -1 )
      {
        printf( "IndexList::readFileInfoHeader(): read src list failed (%s)\n", strerror(errno) );
        return 5;
      }
      
      iError = ::read(fdFile, &segId.typeList[0], segId.typeList.size() * sizeof(segId.typeList[0]) );
      if ( iError == -1 )
      {
        printf( "IndexList::readFileInfoHeader(): read type list failed (%s)\n", strerror(errno) );
        return 6;
      }
    }        
    segId.bSrcUpdated = true;
  }    
  
  int64_t lliOffset = lseek64(fdFile, 0, SEEK_CUR);
  unsigned int uHeaderPadding = 0;
  if (lliOffset % 4 != 0)
    uHeaderPadding =  (4 - lliOffset % 4);    
  printf( "Read Header Padding %d bytes\n", uHeaderPadding );
  if ( uHeaderPadding > 0 )
  {    
    uint8_t lcPadding[uHeaderPadding];    
    int iError = ::read(fdFile, &lcPadding, sizeof(lcPadding) );
    if ( iError == -1 )
      printf( "IndexList::readFileInfoHeader(): read header padding failed (%s)\n", strerror(errno) );           
  }    

  return 0;
}

int IndexList::readFileMainContent(int fdFile, int iNumIndex)
{  
  _lNode.clear();  
  for ( int iNode = 0; iNode < iNumIndex; iNode++ )
  {
    IndexFileL1NodeType fileNode;

    int iError = ::read(fdFile, &fileNode, sizeof(fileNode) );
    if ( iError == -1 )
      printf( "IndexList::readFileMainContent(): read node %d failed (%s)\n", iNode, strerror(errno) );              

    L1AcceptNode l1Node(fileNode);
    _lNode.push_back(l1Node);
  }
  
  return 0;  
}

/*
 * class L1SegmentIndex
 */ 
L1SegmentIndex::L1SegmentIndex(const ProcInfo& procNode1) :
  procNode(procNode1)
{
}

bool L1SegmentIndex::operator<(const L1SegmentIndex& right) const
{
  if ( procNode.ipAddr() == right.procNode.ipAddr() )
    return ( procNode.processId() < right.procNode.processId() );
  else 
    return ( procNode.ipAddr() < right.procNode.ipAddr() );
}

/*
 * class L1SegmentId
 */ 
L1SegmentId::L1SegmentId(int iIndex1) : 
  iIndex(iIndex1), bSrcUpdated(false)
{
}
 
/*
 * class L1AcceptNode
 */ 
L1AcceptNode::L1AcceptNode(uint32_t uSeconds1, uint32_t uNanoseconds1, uint32_t uFiducial1, int64_t lliOffset1) :
  uSeconds(uSeconds1), uNanoseconds(uNanoseconds1), uFiducial(uFiducial1), lliOffsetXtc(lliOffset1), 
  damage(0), uMaskDetDmgs(0), uMaskDetData(0), uMaskEvrEvents(0)
{
}

L1AcceptNode::L1AcceptNode(IndexFileL1NodeType& fileNode) :
  uSeconds      (fileNode.uSeconds),
  uNanoseconds  (fileNode.uNanoseconds),
  uFiducial     (fileNode.uFiducial), 
  lliOffsetXtc  (fileNode.lliOffsetXtc), 
  damage        (fileNode.damage),
  uMaskDetDmgs  (fileNode.uMaskDetDmgs),
  uMaskDetData  (fileNode.uMaskDetData),
  uMaskEvrEvents(fileNode.uMaskEvrEvents)
{
}

bool L1AcceptNode::laterThan(const L1AcceptNode& node)
{
  if ( uSeconds > node.uSeconds )           return true;
  if ( uSeconds < node.uSeconds )           return false;
  if ( uNanoseconds > node.uNanoseconds )   return true;
  if ( uNanoseconds < node.uNanoseconds )   return false;
  if ( lliOffsetXtc >= node.lliOffsetXtc )  return true;
  
  return false;
}

} // namespace Index
} // namespace Pds

