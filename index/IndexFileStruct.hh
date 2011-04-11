#ifndef Pds_Index_IndexFileStruct_hh
#define Pds_Index_IndexFileStruct_hh

#include "pdsdata/xtc/Damage.hh"
#include "pdsdata/xtc/Xtc.hh"

namespace Pds
{  
namespace Index
{
  
#pragma pack(1)

/**
 *  Layout of Index File strcture V1
 *
 *  Overview
 *  ========
 *
 *  +----------------------------------------+
 *  |  Header                                |
 *  |    Type: IndexFileHeaderV1             |
 *  +----------------------------------------+
 *  |  List of Segment Information           |
 *  |    (Size defined in header)            |
 *  |    Type: ProcInfo                      |
 *  +----------------------------------------+
 *  |  List of BeginCalibCycle Infomration   |
 *  |    (Size defined in header)            |
 *  |    Type: CalibNode                     |
 *  +----------------------------------------+
 *  |  List of L1Accept Basic Infomation     |
 *  |    (Size defined in header)            |
 *  |    Type: IndexFileL1NodeV1             |
 *  +----------------------------------------+
 *  |  List of L1Accept Extended Infomation  |
 *  |    (Size defined in header)            |
 *  |    Type: Defined later                 |
 *  +----------------------------------------+
 *
 *  Header
 *  ======
 *    Type: IndexFileHeaderV1
 *    Content:
 *      Xtc   xtcIndex;
 *      char  sXtcFilename[iMaxFilenameLen];     
 *      int   iNumIndex;
 *      int   iNumDetector;  
 *      int   iNumCalib; 
 *    Note:
 *      - xtcIndex is a regular xtc object, whose typeid == Id_Index,
 *        and extent == size of the index file
 *
 *  List of Segment Information
 *  ===========================
 *    Type: ProcInfo 
 *    Content:
 *      - see the definition of pdsdata/xtc/ProcInfo.hh
 *    Note:
 *      - The size of list is defined in header.iNumDetector
 *      - It is a list of all segment node's ProcInfo, sorted in IP/Pid numerical order
 *      - This list is used as the reference
 *        - Later the damage mask will use a bit to specify which segment has "Damage", 
 *            and the bit index is referred to the order in this list
 *        - Later the SegDmg object will also store a segment index that has "Damage",
 *            and the index is also referred to the order in this list
 *
 *  List of BeginCalibCycle Infomration
 *  ===================================
 *    Type: CalibNode
 *    Content: 
 *        int64_t lliOffset;  // offset in the xtc file for jumping to this BeginCalibCycle
 *        int32_t iL1Index;   // index number of the first L1Accept event in this Calib Cycle
 *    Note:
 *      - The size of list is defined in header.iNumCalib
 *      - It is a list of all BeginCalibCycle addresses in the xtc file
 *
 *  List of L1Accept Basic Infomation
 *  =========================================
 *    Type: IndexFileL1NodeV1
 *    Content:
 *      uint32_t  uFiducial;    // fiducial of this L1Accept event
 *      int64_t   lliOffsetXtc; // offset in the xtc file for jumping to this event
 *      uint32_t  uOffsetExt;   // offset in this index file for jumping to the extended information
 *      Damage    damage;       // "overall" damage of this event, extracted from L1Accept event's Xtc object
 *      uint32_t  uDetDmgMask;  // bit mask for listing which segment node has "Damage"
 *      uint32_t  uAttribute;   // bit mask showing some logic information for this event. See below.
 *
 *      enum EAttribute         // bit definition for uAttribute
 *      {
 *        BIT_EPICS     = 0,    // If this L1Accept contains EPICS data (from epicsArch) or not
 *        BIT_PRINCETON = 1,    // If this L1Accept contains Princeton frame data or not
 *        BIT_LNK_NEXT  = 16,   // If the next L1Accept event has the same fiducial as this one  
 *        BIT_LNK_PREV  = 17    // If the previous L1Accept event has the same fiducial as this one  
 *      };
 *    Note:
 *      - The size of list is defined in header.iNumIndex
 *      - It is a list of all L1Accept event's basic information
 *      - Each L1Accept event will have a corresponding extended information,
 *        and uOffsetExt is used to locate the extended information in this index file
 *
 *  List of L1Accept Extended Infomation
 *  ====================================
 *    Type: Defined as follows
 *    Content:
 *      uint8_t   uNumEventCodes;             // number of evr event codes in this L1Accept event
 *      uint8_t   lEventCode[uNumEventCodes]; // list of evr event codes
 *      uint8_t   uNumDamages;                // number of segment damages in this L1Accept event
 *      SegDmg    lSegDmg[uNumDamages];       // list of damages, defined as follows:
 *
 *        struct SegDmg
 *        {
 *          uint8_t index;                    // index of the damaged segment node. See "List of Segment Information"
 *          Damage  damage;                   
 *        }
 *    Note:
 *      - The size of list is defined in header.iNumIndex
 *      - It is a list of all L1Accept event's extended information, so each of them
 *          corresponds to an L1Accept basic information, defined above
 *
 */

struct CalibNode
{
  int64_t lliOffset;  // offset in the xtc file for jumping to this BeginCalibCycle
  int32_t iL1Index;   // index number of the first L1Accept event in this Calib Cycle
  
  CalibNode() {}
  CalibNode(int64_t lliOffset1, int32_t iL1Index1) : lliOffset(lliOffset1), iL1Index(iL1Index1) {}
};

class IndexList; // forward declaration

struct IndexFileHeaderV1
{
  static const int iXtcIndexVersion = 1;    
  static const int iMaxFilenameLen  = 32;  
  
  Xtc   xtcIndex;
  char  sXtcFilename[iMaxFilenameLen];     
  int   iNumIndex;
  int   iNumDetector;  
  int   iNumCalib;
  
  IndexFileHeaderV1();
  IndexFileHeaderV1(const IndexList& list);
};

class L1AcceptNode; //forward declaration

struct IndexFileL1NodeV1
{
  uint32_t  uFiducial;    // fiducial of this L1Accept event
  int64_t   lliOffsetXtc; // offset in the xtc file for jumping to this event
  uint32_t  uOffsetExt;   // offset in this index file for jumping to the extended information
  Damage    damage;       // "overall" damage of this event, extracted from L1Accept event's Xtc object
  uint32_t  uDetDmgMask;  // bit mask for listing which segment node has "Damage"
  uint32_t  uAttribute;   // bit mask showing some logic information for this event. See below.
  
  enum EAttribute         // bit definition for uAttribute
  {
    BIT_EPICS     = 0,    // If this L1Accept contains EPICS data (from epicsArch) or not
    BIT_PRINCETON = 1,    // If this L1Accept contains Princeton frame data or not
    BIT_LNK_NEXT  = 16,   // If the next L1Accept event has the same fiducial as this one  
    BIT_LNK_PREV  = 17    // If the previous L1Accept event has the same fiducial as this one  
  };
    
  IndexFileL1NodeV1();
  IndexFileL1NodeV1(const L1AcceptNode& node);
  
  bool hasEpics    () const;
  bool hasPrinceton() const;
  bool linkToNext  () const;
  bool linkToPrev  () const;  
};

struct SegDmg
{
  uint8_t index;
  Damage  damage;
  
  SegDmg(uint8_t index1, Damage& damage1) :index(index1), damage(damage1) {}
  SegDmg() : damage(0) {}
};

typedef IndexFileHeaderV1 IndexFileHeaderType;
typedef IndexFileL1NodeV1 IndexFileL1NodeType;

#pragma pack()

} // namespace Index
} // namespace Pds

#endif // #ifndef Pds_Index_IndexFileStruct_hh
