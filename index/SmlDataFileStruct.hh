#ifndef Pds_SmlData_SmlDataFileStruct_hh
#define Pds_SmlData_SmlDataFileStruct_hh

#include "pdsdata/xtc/Damage.hh"
#include "pdsdata/xtc/Xtc.hh"

namespace Pds
{  
namespace SmlData
{
  
#pragma pack(1)

/**
 *  Layout of SmlData File structure V1
 *
 *  Overview
 *  ========
 *
 *  +----------------------------------------+
 *  |  Header                                |
 *  |    Type: SmlDataFileHeaderV1             
 *  +----------------------------------------+
 *  |   (Largest part of the file)           |
 *  |  List of L1Accept Infomation           |
 *  |    (Size defined in header)            |
 *  |    Type: SmlDataFileL1NodeV1             |
 *  +----------------------------------------+
 *  |  List of BeginCalibCycle Infomration   |
 *  |    (Size defined in header)            |
 *  |    Type: CalibNode                     |
 *  +----------------------------------------+
 *  |  List of Evr Event Codes               |
 *  |    (Size defined in header)            |
 *  |    Type: uint8_t                       |
 *  +----------------------------------------+
 *  |  List of Segment Information           |
 *  |    (Size defined in header)            |
 *  |    Type: See below                     |
 *  +----------------------------------------+
 *
 *  Header
 *  ======
 *    Type: SmlDataFileHeaderV1
 *    Content:
 *      Xtc       xtcIndex;                       // Regular xtc struture
 *      char      sXtcFilename[iMaxFilenameLen];  // Filename of the corresponding xtc file
 *      int32_t   iNumCalib;                      // Number of BeginCalibCycles 
 *      int16_t   iNumEvrEvents;                  // Number of different Evr event codes
 *      int16_t   iNumDetector;                   // Number of detectors (segment level programs)
 *      int32_t   iNumIndex;                      // Number of L1Accept events 
 *      int32_t   iNumOutOrder;                   // Number of out-of-order events
 *      int32_t   iNumOverlapPrev;                // Number of overlapping events with the previous chunk
 *      int32_t   iNumOverlapNext;                // Number of overlapping events with the next chunk (not used yet) 
 *    Note:
 *      - This file structure is padded to 4-bytes memory boundary, so the 
 *        "List of L1Accept Information" is guaranteed to align with 4-bytes boundaries
 *
 *  List of BeginCalibCycle Infomration
 *  ===================================
 *    Type: CalibNode
 *    Content: 
 *      int64_t   i64Offset;      // offset in the xtc file for jumping to this BeginCalibCycle
 *      int32_t   iL1Index;       // index number of the first L1Accept event in this Calib Cycle
 *      uint32_t  uSeconds;       // timestamp (seconds) from Evr 
 *      uint32_t  uNanoseconds;   // timestamp (nanoseconds) from Evr 
 *    Note:
 *      - The size of list is defined in header.iNumCalib
 *      - It is a list of all BeginCalibCycle addresses in the xtc file
 *
 *  List of Evr Event Codes
 *  ===========================
 *    Type: uint8_t 
 *    Content:
 *      uint8_t uEventCode
 *    Note:
 *      - The size of list is defined in header.iNumEvrEvents
 *      - It is a list of all Evr event codes that appear in the current xtc file,
 *          and the order is used to define the bit index in the "uMaskEvrEvents" field of each L1Accept node
 *
 *  List of Segment Information
 *  ===========================
 *    Type: See below
 *    Content: 
 *      Each segment is made up of the following structure:
 *
 *  +----------------------------------------+
 *  |  NodeId                                |
 *  |    Type: ProcInfo                      |
 *  +----------------------------------------+
 *  |  Number of sources in this segment     |
 *  |    Type: uint8_t                       |
 *  +----------------------------------------+
 *  |  List of sources                       |
 *  |    Size: Defined above                 |
 *  |    Type: Src                           |
 *  +----------------------------------------+
 *  |  List of types                         |
 *  |    Size: Defined above                 |
 *  |    Type: TypeId                        |
 *  +----------------------------------------+
 *
 *    Note:
 *      - The size of list is defined in header.iNumDetector
 *      - It is a list of all segment node's information, including the ProcInfo (IP address/PID) and 
 *          the sources/types provided by each segment
 *      - This list is used as the reference
 *        - Later the "uMaskDetDmgs" field will use a bit to specify which segment has "Damage", 
 *            and the bit index is referred to the order in this list
 *        - Later the "uMaskDetData" field will use a bit to specify which segment has "Non-empty data", 
 *            and the bit index is referred to the order in this list
 *
 *  List of L1Accept Infomation
 *  =========================================
 *    Type: SmlDataFileL1NodeV1
 *    Content:
 *      uint32_t  uSeconds;       // timestamp (seconds) from Evr 
 *      uint32_t  uNanoseconds;   // timestamp (nanoseconds) from Evr 
 *      uint32_t  uFiducial;      // fiducial of this L1Accept event
 *      int64_t   i64OffsetXtc;   // offset in the xtc file for jumping to this event
 *      Damage    damage;         // "overall" damage of this event, extracted from L1Accept event's Xtc object
 *      uint32_t  uMaskDetDmgs;   // bit mask for listing which segment node has "Damage"
 *      uint32_t  uMaskDetData;   // bit mask for listing non-empty detector data
 *      uint32_t  uMaskEvrEvents; // bit mask for listing Evr event codes
 *    Note:
 *      - The size of list is defined in header.iNumIndex
 *      - It is a list of all L1Accept event's information
 */

class SmlDataList; // forward declaration

struct SmlDataFileHeaderV1
{
  static const int iXtcSmlDataVersion = 1;
  static const int iMaxFilenameLen  = 32;  
  
  TypeId    typeId;                         // Type Id. Format: (TypeId::Id_SmlData, iXtcSmlDataVersion) 
  char      sXtcFilename[iMaxFilenameLen];  // Filename of the corresponding xtc file
  int32_t   iNumCalib;                      // Number of BeginCalibCycles 
  int16_t   iNumEvrEvents;                  // Number of different Evr event codes
  int16_t   iNumDetector;                   // Number of detectors (segment level programs)
  int32_t   iNumIndex;                      // Number of L1Accept events   
  int32_t   iNumOutOrder;                   // Number of out-of-order events
  int32_t   iNumOverlapPrev;                // Number of overlapping events with the previous chunk
  int32_t   iNumOverlapNext;                // Number of overlapping events with the next chunk (not used yet)
  
  SmlDataFileHeaderV1() {}
  SmlDataFileHeaderV1(const SmlDataList& list);
  SmlDataFileHeaderV1(const SmlDataFileHeaderV1& headerV1);
};

class L1AcceptNode; //forward declaration

struct SmlDataFileL1NodeV1
{
  uint32_t  uSeconds;       // timestamp (seconds) from Evr 
  uint32_t  uNanoseconds;   // timestamp (nanoseconds) from Evr 
  uint32_t  uFiducial;      // fiducial of this L1Accept event
  int64_t   i64OffsetXtc;   // offset in the xtc file for jumping to this event
  Damage    damage;         // "overall" damage of this event, extracted from L1Accept event's Xtc object
  uint32_t  uMaskDetDmgs;   // bit mask for listing which segment node has "Damage"
  uint32_t  uMaskDetData;   // bit mask for listing non-empty detector data
  uint32_t  uMaskEvrEvents; // bit mask for listing Evr event codes
    
  SmlDataFileL1NodeV1() : damage(0) {}
  SmlDataFileL1NodeV1(const L1AcceptNode& node);  

  uint64_t time() const {return ((uint64_t)uSeconds<<32) | uNanoseconds;}
};

struct CalibNode
{
  int64_t   i64Offset;      // offset in the xtc file for jumping to this BeginCalibCycle
  int32_t   iL1Index;       // index number of the first L1Accept event in this Calib Cycle
  uint32_t  uSeconds;       // timestamp (seconds) from Evr 
  uint32_t  uNanoseconds;   // timestamp (nanoseconds) from Evr 
  
  CalibNode() {}
  CalibNode(int64_t i64Offset1, int32_t iL1Index1, uint32_t uSeconds1, uint32_t uNanoseconds1) : 
    i64Offset(i64Offset1), iL1Index(iL1Index1), uSeconds(uSeconds1), uNanoseconds(uNanoseconds1) {}
  uint64_t time() const {return ((uint64_t)uSeconds<<32) | uNanoseconds;}
};

typedef SmlDataFileHeaderV1 SmlDataFileHeaderType;
typedef SmlDataFileL1NodeV1 SmlDataFileL1NodeType;

int convertTimeStringToSeconds(const char* sTime, uint32_t& uSeconds, uint32_t& uNanoseconds);

#pragma pack()

} // namespace SmlData
} // namespace Pds

#endif // #ifndef Pds_SmlData_SmlDataFileStruct_hh
