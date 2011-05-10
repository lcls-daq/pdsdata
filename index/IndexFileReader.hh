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

  int numL1Event        (int& iNumL1Event) const;
  int detectorList      (int& iNumDetector, const ProcInfo*& lDetector) const;
  int srcList           (int iDetector, int& iNumSrc, const Src*& lSrc) const;
  int typeList          (int iDetector, int& iNumType, const TypeId*& lType) const;
  int calibCycleList    (int& iNumCalib   , const CalibNode*& lCalib) const;
  int numL1EventInCalib (int iCalib       , int& iNumL1Event) const;
  int eventLocalToGlobal(int iCalib, int iEvent, int& iGlobalEvent) const;
  int eventGlobalToLocal(int iGlobalEvent, int& iCalib, int& iEvent) const;
  int eventTimeToGlobal (uint32_t uSeconds, uint32_t uNanoseconds, int& iGlobalEvent, bool& bExactMatch, bool& bOvertime);
  int eventTimeToLocal  (uint32_t uSeconds, uint32_t uNanoseconds, int& iCalib, int& iEvent, bool& bExactMatch, bool& bOvertime);
  
  int gotoEvent       (int iCalib, int iEvent, int fdXtc, int& iGlobalEvent);
  int gotoTime        (uint32_t uSeconds, uint32_t uNanoseconds, int fdXtc, int& iGlobalEvent, bool& bExactMatch, bool& bOvertime);
  
  int time            (int iEvent, uint32_t& uSeconds, uint32_t& uNanoseconds);
  int fiducial        (int iEvent, uint32_t& uFiducial);
  int checkEpics      (int iEvent, bool& bEpics);
  int checkPrinceton  (int iEvent, bool& bPrinceton);
  int damage          (int iEvent, Damage& damage);
  int detDmgMask      (int iEvent, uint32_t& uMaskDetDmgs);
  int detDataMask     (int iEvent, uint32_t& uMaskDetData);
  int evrEventList    (int iEvent, unsigned int& uNumEvent, const uint8_t*& lEvrEvent);

private:
  int gotoL1Node      (int iL1Node);
  
  typedef std::vector<TypeId>     TTypeList;
  typedef std::vector<Src>        TSrcList;
  typedef std::vector<TTypeList>  TListOfTypeList;
  typedef std::vector<TSrcList>   TListOfSrcList;
  
  FILE*               _fXtcIndex;
  IndexFileHeaderV1   _fileHeader;
  int                 _iHeaderSize;
  std::vector<CalibNode>
                      _lCalib;
  std::vector<uint8_t>
                      _lEvrEvent;
  std::vector<ProcInfo>
                      _lDetector;
  TListOfTypeList     _lTypeList;
  TListOfSrcList      _lSrcList;
  
  int                 _iCurL1Node;
  IndexFileL1NodeV1   _curL1Node;  
    
  std::vector<uint8_t>
                      _lCurEvrEvent;
                      
};

#pragma pack()

} // namespace Index
} // namespace Pds

#endif // #ifndef Pds_Index_IndexFileReader_hh
