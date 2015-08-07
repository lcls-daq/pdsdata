#include <string>

#include <unistd.h>
#include <errno.h>
#include <assert.h>
#include <sys/stat.h>

#include "pdsdata/index/SmlDataChunkReader.hh"
#include "pdsdata/index/SmlDataFileReader.hh"

using std::string;

namespace Pds
{  
namespace SmlData
{

SmlDataChunkReader::SmlDataChunkReader() : _bValid(false), _iNumTotalL1Event(0)
{
}

SmlDataChunkReader::~SmlDataChunkReader()
{
  close();
}

int SmlDataChunkReader::open(const char* sFnIndex)
{
  if (sFnIndex == NULL)
  {
    printf("SmlDataChunkReader::open(): Invalid argument: No filename specified\n");
    return 1;
  }
  
  /*
   * Get base filename
   */
  string  strFnIndex(sFnIndex);  
  bool    bSingleIndex = false;  
  size_t  uPos = strFnIndex.find("-c");
  if (uPos == string::npos)
    bSingleIndex = true;
  
  close();

  if (bSingleIndex)
  {
    _strFnBase = strFnIndex;
    
    struct ::stat64 statFile;
    int iError = ::stat64(_strFnBase.c_str(), &statFile);
    if ( iError != 0 )
    {
      // test if the file is under the "index" sub-dir      
      size_t iFindDir = strFnIndex.rfind("/");
      if (iFindDir == std::string::npos )
        _strFnBase = "index/" + _strFnBase;
      else
        _strFnBase = strFnIndex.substr(0, iFindDir+1) + "index/" + strFnIndex.substr(iFindDir+1);
            
      iError = ::stat64(_strFnBase.c_str(), &statFile);
      if ( iError != 0 )
      {     
        printf("SmlDataChunkReader::open(): No index file exists: %s\n", _strFnBase.c_str());
        return 2;
      }
    }
    
    //printf("SmlDataChunkReader::Reading %s\n", sFnBuf);//!!debug        
    SmlDataFileReader* pIndex = new SmlDataFileReader();
    _lIndex.push_back(pIndex);
    
    pIndex->open(_strFnBase.c_str());
    if (!pIndex->isValid())
    {
      printf("SmlDataChunkReader::open(): Error loading index file %s\n", _strFnBase.c_str());
      return 3;
    }    
  }
  else // if (bSingleIndex)
  {    
    _strFnBase = strFnIndex.substr(0, uPos+2);
    //printf("SmlDataChunkReader::open(): Base %s\n", _strFnBase.c_str());//!!debug  
    
    /*
     * Read index files
     */
    for (int iFileSerial = 0;;++iFileSerial)
    {
      char sFnBuf[128];
      sprintf(sFnBuf, "%s%02d.xtc.idx", _strFnBase.c_str(), iFileSerial);
      
      struct ::stat64 statFile;
      int iError = ::stat64(sFnBuf, &statFile);
      if ( iError != 0 )
      {
        // test if the file is under the "index" sub-dir
        size_t iFindDir = strFnIndex.rfind("/");
        if (iFindDir == std::string::npos )
          sprintf(sFnBuf, "index/%s%02d.xtc.idx", _strFnBase.c_str(), iFileSerial);
        else
        {
          string strIndexDir    = strFnIndex.substr(0, iFindDir+1);
          string strIndexFnOnly = strFnIndex.substr(iFindDir+1);
          sprintf(sFnBuf, "%s/index/%s%02d.xtc.idx", strIndexDir.c_str(), strIndexFnOnly.c_str(), iFileSerial);
        }
        
        iError = ::stat64(sFnBuf, &statFile);
        if ( iError != 0 )
          break;            
      }

      //printf("SmlDataChunkReader::Reading %s\n", sFnBuf);//!!debug
        
      SmlDataFileReader* pIndex = new SmlDataFileReader();
      _lIndex.push_back(pIndex);
      
      pIndex->open(sFnBuf);
      if (!pIndex->isValid())
      {
        printf("SmlDataChunkReader::open(): Error loading index file %s\n", sFnBuf);
        break;
      }
    }
  
  }

  int iError = _buildInfo();
  if (iError != 0)
    return 4;

  _bValid = true;
  return 0;
}

int SmlDataChunkReader::close()
{
  if (!_bValid)
    return 0;
  
  //printf("SmlDataChunkReader::close(): BaseFn %s\n", _strFnBase.c_str());//!!debug
    
  for (TListIndex::iterator it = _lIndex.begin();
    it != _lIndex.end(); ++it )
  {    
    delete (*it);
    *it = NULL;
  }
  
  _lIndex     .clear();
  _lCalib     .clear();

  _bValid = false;
  return 0;
}

bool SmlDataChunkReader::isValid() const 
{
  return _bValid;
}

int SmlDataChunkReader::numL1Event(int& iNumL1Event) const
{
  iNumL1Event = -1;
  if (!_bValid)
    return 1;
    
  iNumL1Event = _iNumTotalL1Event;    
  return 0;
}

int SmlDataChunkReader::detectorList(int& iNumDetector, const ProcInfo*& lDetector) const
{
  iNumDetector = -1;
  if (!_bValid)
    return 1;

  if (_lIndex.size() <= 0)
    return 0;
    
  SmlDataFileReader& index = *(_lIndex[0]);  
  return index.detectorList(iNumDetector, lDetector);
}

int SmlDataChunkReader::srcList(int iDetector, int& iNumSrc, const Src*& lSrc) const
{
  iNumSrc = -1;
  if (!_bValid)
    return 1;
    
  if (_lIndex.size() <= 0)
    return 0;
    
  SmlDataFileReader& index = *(_lIndex[0]);  
  return index.srcList(iDetector, iNumSrc, lSrc);
}

int SmlDataChunkReader::typeList(int iDetector, int& iNumType, const TypeId*& lType) const
{
  iNumType = -1;
  if (!_bValid)
    return 1;
    
  if (_lIndex.size() <= 0)
    return 0;
    
  SmlDataFileReader& index = *(_lIndex[0]);  
  return index.typeList(iDetector, iNumType, lType);
}

int SmlDataChunkReader::numCalibCycle(int& iNumCalib) const
{
  iNumCalib = -1;
  if (!_bValid)
    return 1;
    
  iNumCalib = _lCalib.size();
  return 0;
}

int SmlDataChunkReader::numL1EventInCalib(int iCalib, int& iNumL1Event) const
{
  if (!_bValid)
    return 1;
    
  if (iCalib < 0 || iCalib >= (int) _lCalib.size())
  {
    printf("SmlDataChunkReader::numL1EventInCalib(): Invalid Calib# %d. Max # = %d\n",
      iCalib, (int) _lCalib.size()-1);
    return 1;
  }
    
  const CalibInfo& calibInfo  = _lCalib[iCalib];
  iNumL1Event                 = calibInfo.iNumL1Event;
  return 0;
}

int SmlDataChunkReader::eventCalibToGlobal(int iCalib, int iEvent, int& iGlobalEvent) const
{
  if (!_bValid)
    return 1;

  if (iCalib < 0 || iCalib >= (int) _lCalib.size())
  {
    printf("SmlDataChunkReader::eventCalibToGlobal(): Invalid Calib# %d. Max # = %d\n",
      iCalib, (int) _lCalib.size()-1);
    return 2;
  }
  
  const CalibInfo& calibInfo  = _lCalib[iCalib];
  if (iEvent < 0 || iEvent >= calibInfo.iNumL1Event)
  {
    printf("SmlDataChunkReader::eventCalibToGlobal(): Invalid Event# %d. Max # = %d\n",
      iEvent, calibInfo.iNumL1Event-1);
    return 3;
  }
  
  assert(calibInfo.lChunkCalib.size() > 0);  
  iGlobalEvent = calibInfo.lChunkCalib[0].iGlobalL1Index + iEvent;    
  return 0;
}

int SmlDataChunkReader::eventGlobalToCalib(int iGlobalEvent, int& iCalib, int& iEvent) const
{
  if (!_bValid)
    return 1;
  
  if (iGlobalEvent < 0 || iGlobalEvent >= _iNumTotalL1Event)
  {
    printf("SmlDataChunkReader::eventGlobalToCalib(): Invalid global Event # %d. Max # = %d\n",
      iGlobalEvent, _iNumTotalL1Event-1);
    return 2;
  }
  
  for (iCalib=0; iCalib < (int) _lCalib.size(); ++iCalib)
  {
    const CalibInfo& calibInfo = _lCalib[iCalib];
    
    assert(calibInfo.lChunkCalib.size() > 0);
    int iEventRemain  = iGlobalEvent - calibInfo.lChunkCalib[0].iGlobalL1Index;
    if (iEventRemain < calibInfo.iNumL1Event)
    {
      iEvent = iEventRemain;
      return 0;
    }
  }
  
  printf( "SmlDataChunkReader::eventGlobalToCalib(): Cannot find correct Calib Cycle for global Event# %d (Max # = %d)n",
    iGlobalEvent, _iNumTotalL1Event-1);
  return 3;
}

int SmlDataChunkReader::eventChunkToGlobal(int iChunk, int iEvent, int& iGlobalEvent) const
{
  if (!_bValid)
    return 1;

  if (iChunk < 0 || iChunk >= (int) _lIndex.size())
  {
    printf("SmlDataChunkReader::eventChunkToGlobal(): Invalid Chunk# %d. Max # = %d\n",
      iChunk, (int) _lIndex.size()-1);
    return 2;
  }
  
  int iEventAcc = 0;
  for (int iChunkCheck = 0; iChunkCheck < iChunk; ++iChunkCheck)
  {
    SmlDataFileReader& index = *(_lIndex[iChunkCheck]);
    
    int iNumChunkL1Event = -1;
    int iError = index.numL1Event(iNumChunkL1Event);
    if (iError != 0)    
    {
      printf("SmlDataChunkReader::eventChunkToGlobal(): Failed to read total L1 Event # in Chunk %d\n", iChunkCheck);
      return 1;
    } 
    
    iEventAcc += iNumChunkL1Event;
  }
  
  iGlobalEvent = iEventAcc + iEvent;    
  return 0;
}

int SmlDataChunkReader::eventGlobalToChunk(int iGlobalEvent, int& iChunk, int& iEvent) const
{
  if (!_bValid)
    return 1;
  
  if (iGlobalEvent < 0 || iGlobalEvent >= _iNumTotalL1Event)
  {
    printf("SmlDataChunkReader::eventGlobalToCalib(): Invalid global Event # %d. Max # = %d\n",
      iGlobalEvent, _iNumTotalL1Event-1);
    return 2;
  }
  
  int iEventRemain = iGlobalEvent;
  for (iChunk = 0; iChunk < (int)_lIndex.size(); ++iChunk)
  {
    SmlDataFileReader& index = *(_lIndex[iChunk]);
    
    int iNumChunkL1Event = -1;
    int iError = index.numL1Event(iNumChunkL1Event);
    if (iError != 0)    
    {
      printf("SmlDataChunkReader::eventGlobalToChunk(): Failed to read total L1 Event # in Chunk %d\n", iChunk);
      return 1;
    } 

    if (iEventRemain < iNumChunkL1Event)
    {
      iEvent = iEventRemain;
      return 0;
    }    
    
    iEventRemain -= iNumChunkL1Event;
  }
  
  printf( "SmlDataChunkReader::eventGlobalToCalib(): Cannot find correct Chunk for global Event# %d (Max # = %d)n",
    iGlobalEvent, _iNumTotalL1Event-1);
  return 3;  
}

int SmlDataChunkReader::eventTimeToChunk(uint32_t uSeconds, uint32_t uNanoseconds, int& iChunk, int& iEvent, bool& bExactMatch, bool& bOvertime) const
{
  iChunk        = -1;
  iEvent        = -1;
  bExactMatch   = false;
  bOvertime     = false;
  if (!_bValid)
    return 1;
  
  for (iChunk = 0; iChunk < (int)_lIndex.size(); ++iChunk)
  {
    SmlDataFileReader& index = *(_lIndex[iChunk]);

    int iNumChunkL1Event = -1;
    int iError = index.numL1Event(iNumChunkL1Event);
    if (iError != 0)    
    {
      printf("SmlDataChunkReader::eventTimeToChunk(): Failed to read total L1 Event # in Chunk %d\n", iChunk);
      return 1;
    }
    
    if (iNumChunkL1Event <= 0)
      continue;
        
    uint32_t uSecondsChunk = 0, uNanosecondsChunk = 0;
    iError = index.time(iNumChunkL1Event-1, uSecondsChunk, uNanosecondsChunk);
    if (iError != 0)    
    {
      printf("SmlDataChunkReader::eventTimeToChunk(): Failed to get time for Event# %d in Chunk %d\n", 
        iNumChunkL1Event-1, iChunk);
      return 2;
    }
    
    int iCmp = SmlDataFileReader::compareTime(uSeconds, uNanoseconds, uSecondsChunk, uNanosecondsChunk);
    if (iCmp > 0)
      continue;
    
    // The event with input time is located in this chunk
    iError = index.eventTimeToGlobal(uSeconds, uNanoseconds, iEvent, bExactMatch, bOvertime);
    if (iError !=0)
    {
      printf("SmlDataChunkReader::eventTimeToChunk(): Failed to locate the event with input time in Chunk %d\n", iChunk);
      return 3;
    }
        
    return 0;    
  }
  
  /* 
   * The input time is later than the last event in all chunks.
   *
   * In this case, return an error code, and also set bOvertime = true
   */
  bOvertime = true;
  return 4;
}


int SmlDataChunkReader::eventTimeToGlobal(uint32_t uSeconds, uint32_t uNanoseconds, int& iGlobalEvent, bool& bExactMatch, bool& bOvertime) const
{
  iGlobalEvent  = -1;
  bExactMatch   = false;
  bOvertime     = false;

  if (!_bValid)
    return 1;
    
  int iChunk = -1;
  int iEvent = -1;  
  int iError = eventTimeToChunk( uSeconds, uNanoseconds, iChunk, iEvent, bExactMatch, bOvertime );
  if (iError != 0)
    return 2;
  
  iError = eventChunkToGlobal(iChunk, iEvent, iGlobalEvent);
  if (iError !=0)
  {
    printf("SmlDataChunkReader::eventTimeToGlobal(): Failed to convert Event# %d in Chunk %d to Global Event#\n", 
      iEvent, iChunk);
    return 3;
  }
  
  return 0;  
}

int SmlDataChunkReader::eventTimeToCalib(uint32_t uSeconds, uint32_t uNanoseconds, int& iCalib, int& iEvent, bool& bExactMatch, bool& bOvertime) const
{
  iCalib      = -1;
  iEvent      = -1;
  bExactMatch = false;
  bOvertime   = false;
  
  if (!_bValid)
    return 1;
      
  int iGlobalEvent = -1;
  int iError = eventTimeToGlobal( uSeconds, uNanoseconds, iGlobalEvent, bExactMatch, bOvertime );
  if (iError != 0)
    return 2;
    
  iError = eventGlobalToCalib( iGlobalEvent, iCalib, iEvent );
  if (iError != 0)
  {
    printf("SmlDataChunkReader::eventTimeToCalib(): Failed to convert Global Event# %d to Calib # and Event#\n", 
      iGlobalEvent);
    return 3;
  }
      
  return 0;
}

int SmlDataChunkReader::eventNextFiducialToChunk(uint32_t uFiducial, int iFromGlobalEvent, int& iChunk, int& iEvent)
{
  iChunk        = -1;
  iEvent        = -1;
  if (!_bValid)
    return 1;
    
  int iFromChunk      = -1;
  int iFromChunkEvent = -1;
  int iError = eventGlobalToChunk(iFromGlobalEvent, iFromChunk, iFromChunkEvent); 
  if (iError != 0)
  {
    printf("SmlDataChunkReader::eventNextFiducialToChunk(): Failed to find correct Chunk for global Event # %d\n", iFromGlobalEvent);
    return 2;
  }
      
  for (int iChunkTest = iFromChunk; iChunkTest < (int)_lIndex.size(); ++iChunkTest)
  {
    SmlDataFileReader& index = *(_lIndex[iChunkTest]);

    int iChunkEvent = -1;
    int iError = index.eventNextFiducialToGlobal(uFiducial, iFromChunkEvent, iChunkEvent);
    if (iError == 0)
    {
      iChunk = iChunkTest;
      iEvent = iChunkEvent;
      return 0;
    }
    
    iFromChunkEvent = 0;    
  }
  
  return 1;
}  

int SmlDataChunkReader::eventNextFiducialToGlobal(uint32_t uFiducial, int iFromGlobalEvent, int& iGlobalEvent)
{
  iGlobalEvent  = -1;

  if (!_bValid)
    return 1;
    
  int iChunk = -1;
  int iEvent = -1;  
  int iError = eventNextFiducialToChunk( uFiducial, iFromGlobalEvent, iChunk, iEvent);
  if (iError != 0)
    return 2;
  
  iError = eventChunkToGlobal(iChunk, iEvent, iGlobalEvent);
  if (iError !=0)
  {
    printf("SmlDataChunkReader::eventNextFiducialToGlobal(): Failed to convert Event# %d in Chunk %d to global Event#\n", 
      iEvent, iChunk);
    return 3;
  }
  
  return 0;  
}

int SmlDataChunkReader::eventNextFiducialToCalib(uint32_t uFiducial, int iFromGlobalEvent, int& iCalib, int& iEvent)
{
  iCalib      = -1;
  iEvent      = -1;
  
  if (!_bValid)
    return 1;
      
  int iGlobalEvent = -1;
  int iError = eventNextFiducialToGlobal( uFiducial, iFromGlobalEvent, iGlobalEvent );
  if (iError != 0)
    return 2;
    
  iError = eventGlobalToCalib( iGlobalEvent, iCalib, iEvent );
  if (iError != 0)
  {
    printf("SmlDataChunkReader::eventNextFiducialToCalib(): Failed to convert Global Event# %d to Calib # and Event#\n", 
      iGlobalEvent);
    return 3;
  }
      
  return 0;
}

int SmlDataChunkReader::gotoEvent(int iCalib, int iEvent, int& iChunk, int64_t& i64Offset, int& iGlobalEvent)
{
  iChunk        = -1;
  i64Offset     = -1;
  iGlobalEvent  = -1;
  
  if (!_bValid)
    return 1;
  
  if (iCalib < 0 || iCalib >= (int) _lCalib.size())
  {
    printf("SmlDataChunkReader::gotoEvent(): Invalid Calib# %d. Max # = %d\n", iCalib, (int) _lCalib.size()-1);
    return 2;
  }
  
  if (iEvent == -1)
  {
    const CalibInfo&      calibInfo = _lCalib[iCalib];
    const ChunkCalibNode& calibNode = calibInfo.lChunkCalib[0];
    
    i64Offset     = calibInfo.i64ChunkOffset;
    iChunk        = calibNode.iChunk;
    iGlobalEvent  = calibNode.iGlobalL1Index;
    return 0;
  }
    
  int iError = eventCalibToGlobal(iCalib, iEvent, iGlobalEvent);
  if (iError != 0)
    return 2;
    
  int iChunkEvent = -1;
  iError = eventGlobalToChunk(iGlobalEvent, iChunk, iChunkEvent);
  if (iError != 0)
    return 3;
    
  iError = offset(iGlobalEvent, i64Offset);
  if (iError != 0)
    return 4;
    
  return 0;
}

int SmlDataChunkReader::time(int iGlobalEvent, uint32_t& uSeconds, uint32_t& uNanoseconds)
{
  if (!_bValid)
    return 1;
    
  int iChunk = -1;
  int iEvent = -1;
  int iError = eventGlobalToChunk(iGlobalEvent, iChunk, iEvent);
  
  if (iError != 0)
  {
    printf("SmlDataChunkReader::time(): Failed to find correct Chunk for global Event # %d\n", iGlobalEvent);
    return 2;
  }
  
  SmlDataFileReader& index = *(_lIndex[iChunk]);
  return index.time(iEvent, uSeconds, uNanoseconds);
}

int SmlDataChunkReader::fiducial(int iGlobalEvent, uint32_t& uFiducial)
{
  if (!_bValid)
    return 1;
    
  int iChunk = -1;
  int iEvent = -1;
  int iError = eventGlobalToChunk(iGlobalEvent, iChunk, iEvent);
  
  if (iError != 0)
  {
    printf("SmlDataChunkReader::time(): Failed to find correct Chunk for global Event # %d\n", iGlobalEvent);
    return 2;
  }
  
  SmlDataFileReader& index = *(_lIndex[iChunk]);
  return index.fiducial(iEvent, uFiducial);
}

int SmlDataChunkReader::offset(int iGlobalEvent, int64_t& i64Offset)
{
  i64Offset = -1;
  if (!_bValid)
    return 1;
    
  int iChunk = -1;
  int iEvent = -1;
  int iError = eventGlobalToChunk(iGlobalEvent, iChunk, iEvent); 
  if (iError != 0)
  {
    printf("SmlDataChunkReader::offset(): Failed to find correct Chunk for global Event # %d\n", iGlobalEvent);
    return 2;
  }
  
  SmlDataFileReader& index = *(_lIndex[iChunk]);
  return index.offset(iEvent, i64Offset);  
}

int SmlDataChunkReader::damage(int iGlobalEvent, Damage& damage)
{
  if (!_bValid)
    return 1;
    
  int iChunk = -1;
  int iEvent = -1;
  int iError = eventGlobalToChunk(iGlobalEvent, iChunk, iEvent);
  
  if (iError != 0)
  {
    printf("SmlDataChunkReader::time(): Failed to find correct Chunk for global Event # %d\n", iGlobalEvent);
    return 2;
  }
  
  SmlDataFileReader& index = *(_lIndex[iChunk]);
  return index.damage(iEvent, damage);
}

int SmlDataChunkReader::detDmgMask(int iGlobalEvent, uint32_t& uMaskDetDmgs)
{
  if (!_bValid)
    return 1;
    
  int iChunk = -1;
  int iEvent = -1;
  int iError = eventGlobalToChunk(iGlobalEvent, iChunk, iEvent);
  
  if (iError != 0)
  {
    printf("SmlDataChunkReader::time(): Failed to find correct Chunk for global Event # %d\n", iGlobalEvent);
    return 2;
  }
  
  SmlDataFileReader& index = *(_lIndex[iChunk]);
  return index.detDmgMask(iEvent, uMaskDetDmgs);
}

int SmlDataChunkReader::detDataMask(int iGlobalEvent, uint32_t& uMaskDetData)
{
  if (!_bValid)
    return 1;
    
  int iChunk = -1;
  int iEvent = -1;
  int iError = eventGlobalToChunk(iGlobalEvent, iChunk, iEvent);
  
  if (iError != 0)
  {
    printf("SmlDataChunkReader::time(): Failed to find correct Chunk for global Event # %d\n", iGlobalEvent);
    return 2;
  }
  
  SmlDataFileReader& index = *(_lIndex[iChunk]);
  return index.detDataMask(iEvent, uMaskDetData);
}

int SmlDataChunkReader::evrEventList(int iGlobalEvent, unsigned int& uNumEvent, const uint8_t*& lEvrEvent)
{
  if (!_bValid)
    return 1;
    
  int iChunk = -1;
  int iEvent = -1;
  int iError = eventGlobalToChunk(iGlobalEvent, iChunk, iEvent);
  
  if (iError != 0)
  {
    printf("SmlDataChunkReader::time(): Failed to find correct Chunk for global Event # %d\n", iGlobalEvent);
    return 2;
  }
  
  SmlDataFileReader& index = *(_lIndex[iChunk]);
  return index.evrEventList(iEvent, uNumEvent, lEvrEvent);
}

int SmlDataChunkReader::_buildInfo()
{
  _lCalib.clear();
  
  /*
   * Read BeginCalibCycle info
   */         
  int iGlobalStartIndex = 0;  
   
  TListIndex::iterator itIndex = _lIndex.begin();
  for (int iChunk = 0; iChunk < (int)_lIndex.size(); ++iChunk, ++itIndex)
  {
    SmlDataFileReader& index = *(*itIndex);
    
    int iNumChunkL1Event = -1;
    int iError = index.numL1Event(iNumChunkL1Event);
    if (iError != 0)    
    {
      printf("SmlDataChunkReader::_buildCalibInfo(): Failed to read total L1 Event # in Chunk %d\n", iChunk);
      return 1;
    } 
        
    _iNumTotalL1Event += iNumChunkL1Event;
    
    int               iNumChunkCalib  = 0;
    iError = index.numCalibCycle(iNumChunkCalib);
    if (iError != 0)    
    {
      printf("SmlDataChunkReader::_buildCalibInfo(): Failed to read Calib # in Chunk %d\n", iChunk);
      return 1;
    } 
        
    const CalibNode* lChunkCalib;
    iError = index.calibCycleList(lChunkCalib);
    if (iError != 0)    
    {
      printf("SmlDataChunkReader::_buildCalibInfo(): Failed to read Calib Cycle List in Chunk %d\n", iChunk);
      return 1;
    }     
        
    for (int iCalib=-1; iCalib < iNumChunkCalib; iCalib++)
    {
      int iNumCalibEvent = 0;
      iError = index.numL1EventInCalib(iCalib, iNumCalibEvent);
      if (iError != 0)    
      {
        printf("SmlDataChunkReader::_buildCalibInfo(): Failed to read L1 Event # in Calib# %d in Chunk %d\n", iChunk, iCalib);
        return 3;
      }    
      
      if (iCalib == -1)
        _addEventToPrevCalib(iGlobalStartIndex, iChunk, iNumCalibEvent);
      else      
        _makeNewCalib(iGlobalStartIndex, iChunk, iCalib, iNumCalibEvent, lChunkCalib[iCalib].iL1Index, lChunkCalib[iCalib].i64Offset);    
      iGlobalStartIndex += iNumCalibEvent;      
    }
  }
      
  //!!debug
  //printf("Total events: %d\n", _iNumTotalL1Event);
  //_listCalib();  
  
  return 0;
}

int SmlDataChunkReader::_addEventToPrevCalib(int iGlobalStartIndex, int iChunk, int iNumCalibEvent)
{
  if (iNumCalibEvent == 0)
    return 0;
    
  CalibInfo& calibInfo      =   _lCalib.back();
  calibInfo.iNumL1Event     +=  iNumCalibEvent;
  
  ChunkCalibNode& calibNodePrev = calibInfo.lChunkCalib.back();
  calibInfo.lChunkCalib.resize(calibInfo.lChunkCalib.size()+1);  
  
  ChunkCalibNode& calibNode = calibInfo.lChunkCalib.back();  
  calibNode.iGlobalL1Index  = iGlobalStartIndex;
  calibNode.iChunk          = iChunk;
  calibNode.iChunkCalib     = -1;
  calibNode.iCalibL1Index   = calibNodePrev.iCalibL1Index + calibNodePrev.iChunkNumL1Event;
  calibNode.iChunkNumL1Event= iNumCalibEvent;
  
  return 0;
}

int SmlDataChunkReader::_makeNewCalib(int iGlobalStartIndex, int iChunk, int iCalib, int iNumCalibEvent, int iChunkStartIndex, int64_t i64ChunkOffset)
{
  _lCalib.resize(_lCalib.size()+1);
  
  CalibInfo& calibInfo      = _lCalib.back();
  calibInfo.iNumL1Event     = iNumCalibEvent;
  calibInfo.iChunkL1Index   = iChunkStartIndex;
  calibInfo.i64ChunkOffset  = i64ChunkOffset;
  calibInfo.lChunkCalib.resize(1);
  
  ChunkCalibNode& calibNode = calibInfo.lChunkCalib.back();
  calibNode.iGlobalL1Index  = iGlobalStartIndex;
  calibNode.iChunk          = iChunk;
  calibNode.iChunkCalib     = iCalib;
  calibNode.iCalibL1Index   = 0;
  calibNode.iChunkNumL1Event= iNumCalibEvent;
  
  return 0;
}

int SmlDataChunkReader::_listCalib()
{
  printf("Listing %d Calibs\n", (int) _lCalib.size());
  
  for (int iCalib=0; iCalib<(int)_lCalib.size(); iCalib++)
  {
    const CalibInfo& calibInfo = _lCalib[iCalib];
    printf("Calib[%d] Event# %d ChunkIdx %d Off 0x%Lx Chunks: %d\n",
      iCalib, calibInfo.iNumL1Event, calibInfo.iChunkL1Index, (long long) calibInfo.i64ChunkOffset, (int) calibInfo.lChunkCalib.size());
      
    for (int iCalibChunk=0; iCalibChunk<(int)calibInfo.lChunkCalib.size(); iCalibChunk++)
    {
      const ChunkCalibNode& calibNode = calibInfo.lChunkCalib[iCalibChunk];
      printf("  Chunk[%d] Calib %d CalibIdx %d Event# %d GblIdx %d\n",
        calibNode.iChunk, calibNode.iChunkCalib, calibNode.iCalibL1Index, calibNode.iChunkNumL1Event, calibNode.iGlobalL1Index);
    }
  }
  
  return 0;
}

}//namespace SmlData
}//namespace Pds
