#ifndef Pds_Index_IndexFileReader_hh
#define Pds_Index_IndexFileReader_hh

#include <stdio.h>
#include <vector>

#include "pdsdata/xtc/ProcInfo.hh"

#include "IndexFileStruct.hh"

namespace Pds
{  
namespace Index
{
  
#pragma pack(1)
class IndexFileReader
{
public:
      IndexFileReader ();
      ~IndexFileReader();  
  
  int   open          (const char* sXtcIndex);
  int   close         ();
  bool  isValid       () const; 

  int numL1Event      (int& uNumL1Event) const;
  int detectorList    (int& iNumDetector, const ProcInfo*& lDetector) const;
  int calibCyleList   (int& iNumCalib, const CalibNode*& lCalib) const;
  
  int gotoEvent       (int iEventL1Accept, int fdXtc);
  int gotoCalibCycle  (int iCalib, int fdXtc, int& iEventAfterSeek);
  
  int fiducial        (int iEventL1Accept, uint32_t& uFiducial, bool& bLnkNext, bool& bLnkPrev);
  int checkEpics      (int iEventL1Accept, bool& bEpics);
  int checkPrinceton  (int iEventL1Accept, bool& bPrinceton);
  int damageSummary   (int iEventL1Accept, Damage& damage);
  int detDmgMask      (int iEventL1Accept, uint32_t& uDamageMask);
  int evrEventList    (int iEventL1Accept, unsigned int& uNumEvent, const uint8_t*& lEvrEvent);
  int damageList      (int iEventL1Accept, unsigned int& uNumDamage, const SegDmg*& lDamage);

private:
  int gotoL1Node      (int iL1Node);
  int readNodeExtData();
  
  FILE*               _fXtcIndex;
  IndexFileHeaderV1   _fileHeader;
  int                 _iHeaderSize;
  std::vector<ProcInfo>
                      _lDetector;
  std::vector<CalibNode>
                      _lCalib;
  
  int                 _iCurL1Node;
  IndexFileL1NodeV1   _curL1Node;  
    
  bool                _bExtDataRead;
  uint8_t             _iCurNumEvrEvent;
  std::vector<uint8_t>
                      _lCurEvrEvent;
                      
  uint8_t             _iCurNumDamage;
  std::vector<SegDmg>
                      _lCurDamage;                      
};

#pragma pack()

} // namespace Index
} // namespace Pds

#endif // #ifndef Pds_Index_IndexFileReader_hh
