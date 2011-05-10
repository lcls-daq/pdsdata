#include "pdsdata/index/IndexList.hh"

namespace Pds
{  
namespace Index
{

/*
 * class IndexFileHeaderV1
 */ 
IndexFileHeaderV1::IndexFileHeaderV1(const IndexList& list) :
  xtcIndex( TypeId(TypeId::Id_Index, iXtcIndexVersion) )
{
  xtcIndex.extent = list._uFileSize;  
  
  strncpy(sXtcFilename, list._sXtcFilename, iMaxFilenameLen-1);    
  sXtcFilename[iMaxFilenameLen-1] = 0;  
  iHeaderSize   = list._iHeaderSize;
  iNumCalib     = list._lCalib.size();
  iNumEvrEvents = list._mapEvrToId.size();
  iNumDetector  = list._iNumSegments;  
  iNumIndex     = list._lNode.size();
}

/*
 * class IndexFileL1NodeV1
 */ 
IndexFileL1NodeV1::IndexFileL1NodeV1(const L1AcceptNode& node) :
  uSeconds        (node.uSeconds),
  uNanoseconds    (node.uNanoseconds),
  uFiducial       (node.uFiducial), 
  lliOffsetXtc    (node.lliOffsetXtc), 
  damage          (node.damage),
  uMaskDetDmgs    (node.uMaskDetDmgs),
  uMaskDetData    (node.uMaskDetData),
  uMaskEvrEvents  (node.uMaskEvrEvents)
{  
}

} // namespace Index
} // namespace Pds
