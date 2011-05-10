#ifndef Pds_Index_IndexList_hh
#define Pds_Index_IndexList_hh

#include <vector>
#include <map>

#include "pdsdata/xtc/Xtc.hh"
#include "pdsdata/xtc/ProcInfo.hh"
#include "pdsdata/xtc/DetInfo.hh"
#include "pdsdata/xtc/Damage.hh"
#include "pdsdata/xtc/TypeId.hh"
#include "pdsdata/xtc/Dgram.hh"

#include "IndexFileStruct.hh"

namespace Pds
{  
namespace Index
{
  
#pragma pack(1)
 
struct L1AcceptNode
{  
  uint32_t            uSeconds;
  uint32_t            uNanoseconds;
  uint32_t            uFiducial;
  int64_t             lliOffsetXtc;
  Damage              damage;
  uint32_t            uMaskDetDmgs;
  uint32_t            uMaskDetData;
  uint32_t            uMaskEvrEvents;
  
  bool                bEpics;
  bool                bPrinceton;
  
  // Previous/Next node has the same fiducial as this node
  bool                bLnkNext;
  bool                bLnkPrev;
    
        L1AcceptNode(uint32_t uSeconds1, uint32_t uNanoseconds1, uint32_t uFiducial1, int64_t lliOffset1);
        L1AcceptNode(IndexFileL1NodeType& fileNode);
  bool  laterThan(const L1AcceptNode& node);
    
  static const uint32_t uInvalidFiducial  = 0x1ffff;
  static const uint32_t uSegDmgNotPresent = 0x100000;
};

struct L1SegmentIndex
{
  ProcInfo procNode;
  
  L1SegmentIndex(const ProcInfo& procNode1);
  bool operator<(const L1SegmentIndex& right) const;
};

struct L1SegmentId
{
  typedef std::vector<Src>    TSrcList;
  typedef std::vector<TypeId> TTypeList;
  
  int       iIndex;
  TSrcList  srcList;
  TTypeList typeList;
  bool      bSrcUpdated;
  
  explicit L1SegmentId(int iIndex1);
};

class IndexList
{
public:
        IndexList  ();
        IndexList  (const char* sXtcFilename);        
        
  /*
   * L1Accept node operations
   */
  int   startNewNode  (const Dgram& dg, int64_t lliOffset, bool& bInvalidData);
  int   updateSegment (const Xtc& xtc);
  int   updateSource  (const Xtc& xtc, bool& bStopUpdate);
  int   updateReporter(const Xtc& xtc, bool& bStopUpdate);
  int   updateEvr     (const Xtc& xtc);
  int   finishNode    (bool bPrint);
  
  int   getNumNode    () const;
  int   getNode       (int iNode, L1AcceptNode*& pNode);
  
  /*
   * BeginCalibCycle operation
   */
  int   addCalibCycle (int64_t lliOffset);

  int   reset         ();    
  int   setXtcFilename(const char* sXtcFilename);
  int   finishList    ();  
  void  printList     (int iVerbose) const;  
  int   writeToFile   (int fdFile) const;  
  int   readFromFile  (int fdFile);  
  
private:    
  static const int iMaxFilenameLen  = IndexFileHeaderType::iMaxFilenameLen;

  typedef   std::vector<CalibNode>        TCalibList;  
  typedef   std::vector<L1AcceptNode>     TNodeList;  
  typedef   std::vector<Damage>           TSegmentDamageMapList;
  typedef   std::map<L1SegmentIndex,L1SegmentId>  
                                          TSegmentToIdMap;
  typedef   std::map<uint32_t,int>  
                                          TEvrEvtToIdMap;

  char                      _sXtcFilename[iMaxFilenameLen];  
  int                       _iNumSegments;
  TSegmentToIdMap           _mapSegToId;
  TCalibList                _lCalib;
  TEvrEvtToIdMap            _mapEvrToId;
  TSegmentToIdMap::iterator _itCurSeg;

  
  TNodeList                 _lNode;  
  bool                      _bNewNode;   
  L1AcceptNode*             _pCurNode;
  TSegmentDamageMapList     _lSegDmgTmp;
  
  int                       _iCurSerial;
  uint32_t                  _uFileSize;
  int                       _iHeaderSize;
  
  int           finishPrevSegmentId();
  L1AcceptNode& checkInNode( L1AcceptNode& nodeNew );  
  
  void          printNode(const L1AcceptNode& node, int iSerial) const;
  
  int           writeFileInfoHeader (int fdFile) const;
  int           writeFileMainContent(int fdFile) const;

  int           readFileInfoHeader  (int fdFile, int& iNumIndex);
  int           readFileMainContent (int fdFile, int iNumIndex);
  
  friend class IndexFileHeaderV1;
}; // class IndexList

#pragma pack()

} // namespace Index
} // namespace Pds

#endif // #ifndef Pds_Index_IndexList_hh 
