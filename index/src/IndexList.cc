#include <errno.h>
#include <libgen.h>

#include "pdsdata/evr/DataV3.hh"

#include "pdsdata/index/IndexList.hh"

namespace Pds
{  
namespace Index
{

static int compareFiducial( uint32_t uFiducial1, uint32_t uFiducial2 );

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
    
  L1AcceptNode  nodeNew(uFiducial, lliOffset);
  L1AcceptNode& nodeCur = checkInNode(nodeNew);
  
  _pCurNode = &nodeCur;
  
  nodeCur.damage = dg.xtc.damage;
  
  if ( _iNumSegments < 0 )
    _lSegDmgTmp.clear();
  else
    _lSegDmgTmp.assign( _iNumSegments, Damage(L1AcceptNode::uSegDmgNotPresent) );  
  
  _bNewNode = true;
  
  return iError;
}

int IndexList::updateSegment(const Xtc& xtc)
{
  if ( ! _bNewNode )  
  {
    printf( "IndexList::updateSegment():: *** new node hasn't been initialized\n" );
    return 1;
  }
      
  const ProcInfo & info = (const ProcInfo &) (xtc.src);
  
  if ( _iNumSegments < 0 )
  {
    int iSegmentIndex = _lSegDmgTmp.size();
    
    if ( iSegmentIndex != (int) _mapSegToId.size() )
    {
      printf( "IndexList::updateSegment():: *** segment index not updated correctly (vec %d map %d)\n", 
      iSegmentIndex, _mapSegToId.size() );
    }
    
    _lSegDmgTmp.push_back( xtc.damage );        
    _mapSegToId[ info ] = iSegmentIndex;
  }
  else
  {
    TSegmentToIdMap::iterator itFind = 
      _mapSegToId.find( info );
      
    if ( itFind == _mapSegToId.end() )
    {
      printf( "IndexList::updateSegment():: *** cannot find config node for "
        "ip 0x%x pid 0x%x\n", info.ipAddr(), info.processId() );
      return 2;
    }     
  
    int iSegmentIndex = itFind->second;
    if ( iSegmentIndex >= (int) _iNumSegments )
    {
      printf( "IndexList::updateSegment():: *** invalid segment index %d (max value %d)\n", 
        iSegmentIndex, _iNumSegments );
      return 3;
    }
    _lSegDmgTmp[iSegmentIndex] = xtc.damage;
  }
  
  if ( xtc.contains.id() != TypeId::Id_Xtc ) // not a normal segment level
    return 4;   
    
  return 0;
}

int IndexList::updateSource(const Xtc& xtc)
{
  if ( ! _bNewNode )  
  {
    printf( "IndexList::updateSource():: *** new node hasn't been initialized\n" );
    return 1;
  }
  
  L1AcceptNode& node = *_pCurNode;
  
  if ( xtc.contains.id() == TypeId::Id_Epics )
    node.bEpics     = true;  
  else if ( xtc.contains.id() == TypeId::Id_PrincetonFrame )
    node.bPrinceton = true;  
  else if ( xtc.contains.id() == TypeId::Id_EvrData )
    updateEvr( xtc );

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
    _pCurNode->lEventCode.push_back(fifoEvent.EventCode);
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

  /*
   * Copy damaged segments to node's segment-damage list (skip un-damaged nodes)
   */
  if ( _iNumSegments < 0 )
  {
    _iNumSegments = _lSegDmgTmp.size();

    if ( _iNumSegments != (int) _mapSegToId.size() )
    {
      printf( "IndexList::finishNode():: *** segment map size not updated correctly (vec %d map %d)\n", 
      _iNumSegments, _mapSegToId.size() );
    }

    int iMapIndex = 0;
    for ( TSegmentToIdMap::iterator 
      iterMap =  _mapSegToId.begin();
      iterMap != _mapSegToId.end();
      iterMap++, iMapIndex++ )
    {
      int iSegIndex = iterMap->second;
      //const ProcInfo& info      = iterMap->first.procNode;
      //printf( "Segment %d [org %d] ip 0x%x pid 0x%x\n", 
      //  iMapIndex, iSegIndex, info.ipAddr(), info.processId()); // !! debug
      
      if ( _lSegDmgTmp[iSegIndex].value() != 0 )
        _pCurNode->lSegDmg.push_back( SegDmg(iMapIndex, _lSegDmgTmp[iSegIndex]) );      
        
      iterMap->second = iMapIndex;
    }      
  } // if ( _iNumSegments < 0 )
  else
  {
    for ( int iIndex = 0; iIndex < _iNumSegments; iIndex++ )
      if ( _lSegDmgTmp[iIndex].value() != 0 )
        _pCurNode->lSegDmg.push_back( SegDmg(iIndex, _lSegDmgTmp[iIndex]) );
  }
  
  if (bPrint)
    printNode(*_pCurNode, _iCurSerial);
      
  _bNewNode = false;   
  _pCurNode = NULL;  
  return 0;
}

int IndexList::finishList()
{  
  unsigned int uOffsetCur = 
    sizeof(IndexFileHeaderType)                       +
    sizeof(ProcInfo)            * _mapSegToId .size() +
    sizeof(CalibNode)           * _lCalib     .size() +
    sizeof(IndexFileL1NodeType) * _lNode      .size() ;
  
  int iNode = 0;
  for (TNodeList::iterator 
    iterNode =  _lNode.begin();
    iterNode != _lNode.end();
    iterNode++, iNode++)
  {
    L1AcceptNode& node = *iterNode;
    node.uOffsetExt = uOffsetCur;
    
    uint8_t uNumEventCodes = node.lEventCode.size();
    uint8_t uNumDamages    = node.lSegDmg   .size();    
    int iNodeExtSize = 
      sizeof(uNumEventCodes)  +
      sizeof(node.lEventCode[0]) * uNumEventCodes +
      sizeof(uNumDamages)     +
      sizeof(node.lSegDmg[0])    * uNumDamages;
      
    uOffsetCur += iNodeExtSize;
  }    
  
  // adjust the size so that the total xtc size is a multiple of 4
  unsigned int uFilePadding = 0;
  if (uOffsetCur % 4 != 0)
    uFilePadding =  (4 - uOffsetCur % 4);
  _uFileSize    = uOffsetCur + uFilePadding;  
  
  return 0;
}

int IndexList::addCalibCycle(int64_t lliOffset)
{  
  _lCalib.push_back(CalibNode(lliOffset,_lNode.size()));
  return 0;
}

void IndexList::printList(int iVerbose) const
{ 
  printf( "XtcFn %s List index# 0x%x (%u)  Segment# %u File size 0x%x (%u)\n",
    _sXtcFilename, _lNode.size(), _lNode.size(), _mapSegToId.size(),
    _uFileSize, _uFileSize );
  
  if ( iVerbose > 0 )
  {
    for ( TSegmentToIdMap::const_iterator 
      iterMap =  _mapSegToId.begin();
      iterMap != _mapSegToId.end();
      iterMap++ )
    {
      const ProcInfo& info = iterMap->first.procNode;
      printf( "Segment %d ip 0x%x pid 0x%x\n", 
        iterMap->second, info.ipAddr(), info.processId());
    }      
    
    int iCalib = 0;
    for ( TCalibList::const_iterator
      iterCalib =  _lCalib.begin();
      iterCalib != _lCalib.end();
      iterCalib++, iCalib++ )
    {
      const CalibNode& calibNode = *iterCalib;
      printf( "Calib %d Off 0x%Lx L1 %d\n", iCalib, calibNode.lliOffset, calibNode.iL1Index );
    }
  }
  
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
  printf( "[%d] Fid 0x%05x OffXtc 0x%Lx OffExt 0x%x Dmg 0x%x ", 
    iSerial, node.uFiducial, node.lliOffsetXtc, node.uOffsetExt, node.damage.value() );
  printf( "Epc %c Prn %c ", (node.bEpics?'Y':'n'),   (node.bPrinceton?'Y':'n') );

  /*
   * print events     
   */
  printf( "Evn%d ", node.lEventCode.size() );
  if ( node.lEventCode.size() != 0 )
    for ( unsigned int uEvent = 0; uEvent < node.lEventCode.size(); uEvent++ )
      printf( "[%d] ", node.lEventCode[uEvent] );
  
  /*
   * print segment damages
   */
  printf( "Dmg%d ", node.lSegDmg.size() );
  if (node.lSegDmg.size() != 0)
    for (L1AcceptNode::TSegmentDamageList::const_iterator
      iterSegDmg =  node.lSegDmg.begin();
      iterSegDmg != node.lSegDmg.end();
      iterSegDmg++ )
      printf( "[%d]0x%x ", (int) iterSegDmg->index, iterSegDmg->damage.value() );
      
  const char* sLink = ( node.bLnkPrev? (node.bLnkNext?"<->":"<-o") : (node.bLnkNext?"o->":"") );
  if ( node.bLnkPrev||node.bLnkNext )
    printf( "Lnk %s\n", sLink );
  else
    printf( "\n" );
}

int IndexList::writeToFile(int fdFile) const
{  
  int iError1 = writeFileInfoHeader (fdFile);
  int iError2 = writeFileMainContent(fdFile);
  int iError3 = writeFileExtended   (fdFile);

  int iError4 = 0;
  
  int64_t lliOffset    = lseek64(fdFile, 0, SEEK_CUR);
  int     iFilePadding = _uFileSize - lliOffset;
  if ( iFilePadding > 0 )
  {
    printf( "Padding %d bytes\n", iFilePadding );
    uint8_t lcPadding[iFilePadding];
    memset( lcPadding, 0, iFilePadding );
    int iError = ::write(fdFile, &lcPadding, iFilePadding );
    if ( iError == -1 )
    {
      printf( "IndexList::writeToFile(): write file padding failed (%s)\n", strerror(errno) );     
      iError4 = 1;
    }
  }
  else if ( iFilePadding < 0 )
  {
    printf( "IndexList::writeToFile(): file size calculation incorrect\n" );     
    iError4 = 2;
  }
  
  int iError = 
    (iError1 != 0 ? 1 : 0) |
    (iError2 != 0 ? 2 : 0) |
    (iError3 != 0 ? 4 : 0) |
    (iError4 != 0 ? 8 : 0) ;
  return iError;
}

int IndexList::readFromFile(int fdFile)
{  
  int iNumIndex = 0;  
  int iError1 = readFileInfoHeader  (fdFile, iNumIndex);  
  if ( iError1 != 0 )
    return 1;    
  
  int iError2 = readFileMainContent (fdFile, iNumIndex);
  int iError3 = readFileExtended    (fdFile);
  
  int iError = 
    (iError2 != 0 ? 2 : 0) |
    (iError3 != 0 ? 4 : 0);
  return iError;
  
  return 0;
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
  _lSegDmgTmp .clear();
  _mapSegToId .clear();
  
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
  int           iCmp     = compareFiducial( nodeNew.uFiducial, nodeLast.uFiducial );
    
  if ( iCmp >= 0 )
  {
    if ( iCmp == 0 )
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
    int iCmp = compareFiducial( nodeNew.uFiducial, itNodeIns->uFiducial );

    if ( iCmp >= 0 )
    {
      if ( iCmp == 0 )
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
    
  IndexFileHeaderType fileHeader(*this);
  iError = ::write(fdFile, &fileHeader, sizeof(fileHeader) );
  if ( iError == -1 )
    printf( "IndexList::writeFileInfoHeader(): write file info header failed (%s)\n", strerror(errno) );
  
  int iInfo = 0;
  for ( TSegmentToIdMap::const_iterator 
    itMap =  _mapSegToId.begin();
    itMap != _mapSegToId.end();
    itMap++, iInfo++ )
  {
    const ProcInfo& info  = (itMap->first).procNode;
    iError = ::write(fdFile, &info, sizeof(info) );
    if ( iError == -1 )
      printf( "IndexList::writeFileInfoHeader(): write proc info failed (%s)\n", strerror(errno) );    
  }

  iError = ::write(fdFile, &_lCalib[0], _lCalib.size() * sizeof(_lCalib[0]) );  
  if ( iError == -1 )
    printf( "IndexList::writeFileInfoHeader(): write calib node failed (%s)\n", strerror(errno) );    
  
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
  
  return 0;
}

int IndexList::writeFileExtended(int fdFile) const
{
  int iError = -1;
  int iNode  = 0;
  
  for (TNodeList::const_iterator 
    iterNode =  _lNode.begin();
    iterNode != _lNode.end();
    iterNode++, iNode++)
  {
    const L1AcceptNode& node = *iterNode;
    
    uint8_t uNumEventCodes = node.lEventCode.size();    
    iError = ::write(fdFile, &uNumEventCodes, sizeof(uNumEventCodes) );
    if ( iError == -1 )
      printf( "IndexList::writeFileExtended(): write node %d event code number failed (%s)\n", iNode, strerror(errno) );
    
    iError = ::write(fdFile, (char*) &(node.lEventCode[0]), sizeof(node.lEventCode[0]) * uNumEventCodes );
    if ( iError == -1 )
      printf( "IndexList::writeFileMainContent(): write node %d event codes failed (%s)\n", iNode, strerror(errno) );            

    uint8_t uNumDamages = node.lSegDmg.size();    
    iError = ::write(fdFile, &uNumDamages, sizeof(uNumDamages) );
    if ( iError == -1 )
      printf( "IndexList::writeFileExtended(): write node %d damage number failed (%s)\n", iNode, strerror(errno) );
    
    iError = ::write(fdFile, (char*) &(node.lSegDmg[0]), sizeof(node.lSegDmg[0]) * uNumDamages );
    if ( iError == -1 )
      printf( "IndexList::writeFileMainContent(): write node %d damages failed (%s)\n", iNode, strerror(errno) );      
  }
  
  return 0;
}

int IndexList::readFileInfoHeader(int fdFile, int& iNumIndex)
{
  IndexFileHeaderType fileHeader;
  
  int iError;
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
  iNumIndex     = fileHeader.iNumIndex;
  strncpy( _sXtcFilename, fileHeader.sXtcFilename, iMaxFilenameLen-1);    
  _sXtcFilename[iMaxFilenameLen-1] = 0;
  
  _iNumSegments = fileHeader.iNumDetector;  
  _mapSegToId.clear();
  for ( int iSeg = 0; iSeg < _iNumSegments; iSeg++ )
  {
    ProcInfo info(Level::Segment,0,0);
    iError = ::read(fdFile, &info, sizeof(info) );
    if ( iError == -1 )
      printf( "IndexList::readFileInfoHeader(): read proc info failed (%s)\n", strerror(errno) );
    _mapSegToId[info] = iSeg;
  }    

  int iNumCalib = fileHeader.iNumCalib;  
  _lCalib.resize(iNumCalib);
  if (iNumCalib > 0)
  {
    iError = ::read(fdFile, &_lCalib[0], _lCalib.size() * sizeof(_lCalib[0]) );
    if ( iError == -1 )
    {
      printf( "IndexList::readFileInfoHeader(): read calib failed (%s)\n", strerror(errno) );
      return 1;
    }
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

int IndexList::readFileExtended(int fdFile)
{
  int iNode;
  for ( TNodeList::iterator
    iterNode =  _lNode.begin();
    iterNode != _lNode.end();
    iterNode++, iNode++
    )
  {
    L1AcceptNode& node = *iterNode;
    
    int iError;
    uint8_t uNumEventCodes;    
    iError = ::read(fdFile, &uNumEventCodes, sizeof(uNumEventCodes) );
    if ( iError == -1 )
      printf( "IndexList::readFileExtended(): read node %d event code number failed (%s)\n", iNode, strerror(errno) );
    
    node.lEventCode.resize(uNumEventCodes);
    iError = ::read(fdFile, (char*) &(node.lEventCode[0]), sizeof(node.lEventCode[0]) * uNumEventCodes );
    if ( iError == -1 )
      printf( "IndexList::readFileExtended(): read node %d event codes failed (%s)\n", iNode, strerror(errno) );            

    uint8_t uNumDamages = node.lSegDmg.size();    
    iError = ::read(fdFile, &uNumDamages, sizeof(uNumDamages) );
    if ( iError == -1 )
      printf( "IndexList::readFileExtended(): read node %d damage number failed (%s)\n", iNode, strerror(errno) );
    
    node.lSegDmg.resize(uNumDamages);
    iError = ::read(fdFile, (char*) &(node.lSegDmg[0]), sizeof(node.lSegDmg[0]) * uNumDamages );
    if ( iError == -1 )
      printf( "IndexList::readFileExtended(): read node %d damages failed (%s)\n", iNode, strerror(errno) );      
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
 * class L1AcceptNode
 */ 
L1AcceptNode::L1AcceptNode(uint32_t uFiducial1, int64_t lliOffset1) :
  uFiducial(uFiducial1), lliOffsetXtc(lliOffset1), uOffsetExt(0), damage(0),
  bEpics(false), bPrinceton(false), bLnkNext(false), bLnkPrev(false)
{
}

L1AcceptNode::L1AcceptNode(IndexFileL1NodeType& fileNode) :
  uFiducial     (fileNode.uFiducial), 
  lliOffsetXtc  (fileNode.lliOffsetXtc), 
  uOffsetExt    (fileNode.uOffsetExt),
  damage        (fileNode.damage),
  bEpics        ((fileNode.uAttribute & (1<<IndexFileL1NodeType::BIT_EPICS)) != 0),
  bPrinceton    ((fileNode.uAttribute & (1<<IndexFileL1NodeType::BIT_PRINCETON)) != 0),
  bLnkNext      ((fileNode.uAttribute & (1<<IndexFileL1NodeType::BIT_LNK_NEXT)) != 0),
  bLnkPrev      ((fileNode.uAttribute & (1<<IndexFileL1NodeType::BIT_LNK_PREV)) != 0)
{
}

static int compareFiducial( uint32_t uFiducial1, uint32_t uFiducial2 )
{
  if ( uFiducial1 == uFiducial2 ) return 0;  
  
  const int iFiducialWrapAroundDiffMin = 65536; 
  if ( uFiducial1 < uFiducial2 && 
    uFiducial2 < uFiducial1 + iFiducialWrapAroundDiffMin // not a fiducial wrap-around
    )
    return -1;
  
  return 1;
}

} // namespace Index
} // namespace Pds

