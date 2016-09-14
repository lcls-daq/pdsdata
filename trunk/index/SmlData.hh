#ifndef Pds_SmlData_SmlDataList_hh
#define Pds_SmlData_SmlDataList_hh

#include <vector>
#include <map>

#include "pdsdata/xtc/Xtc.hh"
#include "pdsdata/xtc/ProcInfo.hh"
#include "pdsdata/xtc/DetInfo.hh"
#include "pdsdata/xtc/Damage.hh"
#include "pdsdata/xtc/TypeId.hh"
#include "pdsdata/xtc/Dgram.hh"

#include "SmlDataFileStruct.hh"

namespace Pds
{  
namespace SmlData
{
  
#pragma pack(1)
 
struct L1AcceptNode
{  
  uint32_t            uSeconds;
  uint32_t            uNanoseconds;
  uint32_t            uFiducial;
  int64_t             i64OffsetXtc;
  Damage              damage;
  uint32_t            uMaskDetDmgs;
  uint32_t            uMaskDetData;
  uint32_t            uMaskEvrEvents;
      
  L1AcceptNode();
  L1AcceptNode(uint32_t uSeconds1, uint32_t uNanoseconds1, uint32_t uFiducial1, int64_t i64Offset1);
  L1AcceptNode(SmlDataFileL1NodeType& fileNode);
  int laterThan(const L1AcceptNode& node);
    
  static const uint32_t uInvalidFiducial  = 0x1ffff;
  static const uint32_t uSegDmgNotPresent = 0x100000;

  uint64_t time() const {return ((uint64_t)uSeconds<<32) | uNanoseconds;}
};

#pragma pack()

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

class SmlDataList
{
public:
        SmlDataList  ();
        SmlDataList  (const char* sXtcFilename);        
        
  /*
   * L1Accept node operations
   */
  int   startNewNode  (const Dgram& dg, int64_t i64Offset, bool& bInvalidData);
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
  int   addCalibCycle (int64_t i64Offset, uint32_t uSeconds, uint32_t uNanoseconds);

  int   reset         (bool bClearL1NodeLast = false);    
  int   setXtcFilename(const char* sXtcFilename);
  int   finishList    ();  
  void  printList     (int iVerbose) const;  
  int   writeToFile   (int fdFile) const;  
  int   readFromFile  (int fdFile);  

  typedef   std::vector<CalibNode>        TCalibList;  
  typedef   std::vector<L1AcceptNode>     TNodeList;  
  typedef   std::vector<Damage>           TSegmentDamageMapList;
  typedef   std::map<L1SegmentIndex,L1SegmentId>  
                                          TSegmentToIdMap;
  typedef   std::map<int,int>             TEvrEvtToIdMap;

private:    
  static const int iMaxFilenameLen  = SmlDataFileHeaderType::iMaxFilenameLen;

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
  
  int                       _iNumOutOrder;
  bool                      _bOverlapChecked;
  L1AcceptNode              _l1NodeLast;
  int                       _iNumOverlapPrev;
  int                       _iNumOverlapNext;
  
  int           finishPrevSegmentId();
  L1AcceptNode& checkInNode( L1AcceptNode& nodeNew );  
  
  void          printNode(const L1AcceptNode& node, int iSerial) const;
  
  int           writeFileHeader       (int fdFile) const;
  int           writeFileL1AcceptList (int fdFile) const;
  int           writeFileSupplement   (int fdFile) const;

  int           readFileHeader        (int fdFile, SmlDataFileHeaderType& fileHeader);
  int           readFileL1AcceptList  (int fdFile, int iNumIndex);
  int           readFileSupplement    (int fdFile, const SmlDataFileHeaderType& fileHeader);
  
  friend class SmlDataFileHeaderV1;

public:
  const TNodeList&  getL1()    const {return _lNode;}
  const TCalibList& getCalib() const {return _lCalib;}
  const TSegmentToIdMap& getSeg() const {return _mapSegToId;}
  const TEvrEvtToIdMap& getEvr() const {return _mapEvrToId;}

}; // class SmlDataList

} // namespace SmlData
} // namespace Pds

#endif // #ifndef Pds_SmlData_SmlDataList_hh 
