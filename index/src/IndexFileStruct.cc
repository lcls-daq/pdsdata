#include "pdsdata/index/IndexList.hh"

namespace Pds
{  
namespace Index
{

/*
 * class IndexFileHeaderV1
 */ 
IndexFileHeaderV1::IndexFileHeaderV1()
{
}

IndexFileHeaderV1::IndexFileHeaderV1(const IndexList& list) :
  xtcIndex( TypeId(TypeId::Id_Index, iXtcIndexVersion) )
{
  xtcIndex.extent = list._uFileSize;  
  
  strncpy(sXtcFilename, list._sXtcFilename, iMaxFilenameLen-1);    
  sXtcFilename[iMaxFilenameLen-1] = 0;  
  iNumIndex     = list._lNode.size();
  iNumDetector  = list._iNumSegments;  
  iNumCalib     = list._lCalib.size();
}

/*
 * class IndexFileL1NodeV1
 */ 
IndexFileL1NodeV1::IndexFileL1NodeV1() : damage(0)
{
}

IndexFileL1NodeV1::IndexFileL1NodeV1(const L1AcceptNode& node) :
  uFiducial     (node.uFiducial), 
  lliOffsetXtc  (node.lliOffsetXtc), 
  uOffsetExt    (node.uOffsetExt),
  damage        (node.damage)
{
  uDetDmgMask = 0;
  
  for (L1AcceptNode::TSegmentDamageList::const_iterator
    iterDmg =  node.lSegDmg.begin();
    iterDmg != node.lSegDmg.end();
    iterDmg++)
  {
    uDetDmgMask |= ( 1<< iterDmg->index );
  }
  
  uAttribute = 
    (node.bEpics    ? (1<<BIT_EPICS    )  : 0 ) |
    (node.bPrinceton? (1<<BIT_PRINCETON)  : 0 ) |
    (node.bLnkNext  ? (1<<BIT_LNK_NEXT )  : 0 ) |
    (node.bLnkPrev  ? (1<<BIT_LNK_PREV )  : 0 ) ;
}

bool IndexFileL1NodeV1::hasEpics() const
{
  return (uAttribute & (1<<BIT_EPICS)) != 0;
}

bool IndexFileL1NodeV1::hasPrinceton() const
{
  return (uAttribute & (1<<BIT_PRINCETON)) != 0;
}

bool IndexFileL1NodeV1::linkToNext() const
{
  return (uAttribute & (1<<BIT_LNK_NEXT)) != 0;
}

bool IndexFileL1NodeV1::linkToPrev() const
{
  return (uAttribute & (1<<BIT_LNK_PREV)) != 0;
}

} // namespace Index
} // namespace Pds
