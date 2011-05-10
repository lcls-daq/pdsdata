#include <errno.h>

#include "pdsdata/index/IndexFileReader.hh"

namespace Pds
{  
namespace Index
{
// forward declaration
static int compareTime(uint32_t& uSeconds1, uint32_t& uNanoseconds1, uint32_t& uSeconds2, uint32_t& uNanoseconds2);
  
IndexFileReader::IndexFileReader() : _fXtcIndex(NULL), _iHeaderSize(0), _iCurL1Node(-1)
{
}

IndexFileReader::~IndexFileReader()
{
  close();
}

int IndexFileReader::open(const char* sXtcIndex)
{
  close();
    
  _fXtcIndex = fopen( sXtcIndex, "rb" );
  
  if ( _fXtcIndex == NULL )
  {
    printf( "IndexFileReader::open(): open index file %s failed, error = %s\n",
      sXtcIndex, strerror(errno) );
    return 1;
  }
  
  int iItemsRead = fread(&_fileHeader, sizeof(_fileHeader), 1, _fXtcIndex);
  if ( iItemsRead != 1 )
  {
    printf( "IndexFileReader::open(): read header failed, error = %s\n",
      strerror(errno) );
    return 2;
  }

  if (_fileHeader.iNumCalib > 0)
  {
    _lCalib.resize(_fileHeader.iNumCalib);
    iItemsRead = fread( (void*) &_lCalib[0], _lCalib.size() * sizeof(_lCalib[0]), 1, _fXtcIndex);
    if ( iItemsRead != 1 )
    {
      printf( "IndexFileReader::open(): read calib cycle (%d) failed, error = %s\n",
        _fileHeader.iNumCalib, strerror(errno) );
      return 3;
    }  
  }

  if (_fileHeader.iNumEvrEvents > 0)
  {
    _lEvrEvent.resize(_fileHeader.iNumEvrEvents);
    iItemsRead = fread( (void*) &_lEvrEvent[0], _lEvrEvent.size() * sizeof(_lEvrEvent[0]), 1, _fXtcIndex);
    if ( iItemsRead != 1 )
    {
      printf( "IndexFileReader::open(): read evr events (%d) failed, error = %s\n",
        _fileHeader.iNumEvrEvents, strerror(errno) );
      return 4;
    }  
  }  
  
  if (_fileHeader.iNumDetector > 0)
  {
    _lDetector.resize(_fileHeader.iNumDetector, ProcInfo(Level::Segment,0,0));
    _lTypeList.resize(_fileHeader.iNumDetector);
    _lSrcList .resize(_fileHeader.iNumDetector);
    
    for ( int iDetector = 0; iDetector < _fileHeader.iNumDetector; ++iDetector )
    {
      iItemsRead = fread( (void*) &_lDetector[iDetector], sizeof(_lDetector[iDetector]), 1, _fXtcIndex);
      if ( iItemsRead != 1 )
      {
        printf( "IndexFileReader::open(): read detector %d procInfo failed, error = %s\n",
          iDetector, strerror(errno) );
        return 5;
      }
      
      uint8_t uNumSrc;
      iItemsRead = fread( (void*)&uNumSrc, sizeof(uNumSrc), 1, _fXtcIndex);
      if ( iItemsRead != 1 )
      {
        printf( "IndexFileReader::open(): read detector src# for detector %d failed, error = %s\n",
          iDetector, strerror(errno) );
        return 6;
      }  

      TSrcList&  lSrc  = _lSrcList [iDetector];
      lSrc.resize(uNumSrc);
      iItemsRead = fread( (void*) &lSrc[0], lSrc.size() * sizeof(lSrc[0]), 1, _fXtcIndex);
      if ( iItemsRead != 1 )
      {
        printf( "IndexFileReader::open(): read src list for detector %d failed, error = %s\n",
          iDetector, strerror(errno) );
        return 8;
      }              
      
      TTypeList& lType = _lTypeList[iDetector];
      lType.resize(uNumSrc);
      iItemsRead = fread( (void*) &lType[0], lType.size() * sizeof(lType[0]), 1, _fXtcIndex);
      if ( iItemsRead != 1 )
      {
        printf( "IndexFileReader::open(): read type list for detector %d failed, error = %s\n",
          iDetector, strerror(errno) );
        return 7;
      }              
    } // for ( int iDetector = 0; iDetector < _fileHeader.iNumDetector; ++iDetector )
  } // if (_fileHeader.iNumDetector > 0)
  
  _iHeaderSize = _fileHeader.iHeaderSize;
  
  return 0;
}

int IndexFileReader::close()
{
  if ( _fXtcIndex == NULL )
    return 0;
    
  int iError = fclose(_fXtcIndex);  
  if ( iError != 0 )
  {
    printf( "IndexFileReader::close(): close index file failed, error = %s\n",
      strerror(errno) );
    return 1;
  }      
  
  _fXtcIndex        = NULL;
  _iHeaderSize      = 0;
  _iCurL1Node       = -1;
  _lCalib   .clear();
  _lEvrEvent.clear();
  _lDetector.clear();
  _lSrcList .clear();
  _lTypeList.clear();
  
  return 0;
}

bool IndexFileReader::isValid() const
{
  return (_fXtcIndex != NULL);
}

int IndexFileReader::numL1Event(int& iNumL1Event) const
{
  iNumL1Event = -1;
  if (_fXtcIndex == NULL)
    return 1;       

  iNumL1Event = _fileHeader.iNumIndex;
  return 0;
}

int IndexFileReader::numL1EventInCalib(int iCalib, int& iNumL1Event) const
{
  iNumL1Event = -1;
  if (_fXtcIndex == NULL)
    return 1;
    
  if ( iCalib < 0 || iCalib >= (int) _lCalib.size() )
  {
    printf( "IndexFileReader::numL1EventInCalib(): Invalid Calib Cyle #%d (Max # = %d)\n",
      iCalib, _lCalib.size()-1 );
    return 2;
  }    
    
  if ( iCalib == (int) _lCalib.size() - 1 )
    iNumL1Event = _fileHeader.iNumIndex       - _lCalib[iCalib].iL1Index;
  else
    iNumL1Event = _lCalib[iCalib+1].iL1Index  - _lCalib[iCalib].iL1Index;
    
  return 0;
}

int IndexFileReader::eventLocalToGlobal(int iCalib, int iEvent, int& iGlobalEvent) const
{
  if (_fXtcIndex == NULL)
    return 1;
  
  if ( iCalib < 0 || iCalib >= (int) _lCalib.size() )
  {
    printf( "IndexFileReader::eventLocalToGlobal(): Invalid Calib Cyle #%d (Max # = %d)\n",
      iCalib, _lCalib.size()-1 );
    return 2;
  }   
  
  int iMaxEventNum; 
  numL1EventInCalib( iCalib, iMaxEventNum );

  if ( iEvent < 0 || iEvent >= (int) iMaxEventNum )
  {
    printf( "IndexFileReader::eventLocalToGlobal(): Invalid Event #%d in Calib Cycle #%d (Max # = %d)\n",
      iEvent, iCalib, iMaxEventNum );
    return 3;
  }   
  
  iGlobalEvent = _lCalib[iCalib].iL1Index + iEvent;      
  return 0;
}

int IndexFileReader::eventGlobalToLocal(int iGlobalEvent, int& iCalib, int& iEvent) const
{
  if (_fXtcIndex == NULL)
    return 1;

  if ( iGlobalEvent < 0 || iGlobalEvent >= _fileHeader.iNumIndex )
  {
    printf( "IndexFileReader::eventGlobalToLocal(): Invalid global Event #%d (Max # = %d)\n",
      iGlobalEvent, _fileHeader.iNumIndex );
    return 2;
  }   
  
  for (iCalib = 0; iCalib < (int) _lCalib.size(); iCalib++)
  {
    int iRemainingEvent = iGlobalEvent - _lCalib[iCalib].iL1Index;
    
    int iMaxEventNum; 
    numL1EventInCalib( iCalib, iMaxEventNum );    
    if ( iRemainingEvent < iMaxEventNum )
    {
      iEvent = iRemainingEvent;
      return 0;
    }
  }
  
  printf( "IndexFileReader::eventGlobalToLocal(): Cannot find correct Calib Cycle for global Event #%d (Max # = %d)\n",
    iGlobalEvent, _fileHeader.iNumIndex );
  return 3;
}

int IndexFileReader::eventTimeToGlobal(uint32_t uSeconds, uint32_t uNanoseconds, int& iGlobalEvent, bool& bExactMatch, bool& bOvertime)
{  
  iGlobalEvent  = -1;
  bExactMatch   = false;
  bOvertime     = false;
  if (_fXtcIndex == NULL)
    return 1;  
  
  if ( _fileHeader.iNumIndex == 0 )
    return 0;
  else if ( _fileHeader.iNumIndex == 1 )
  {
    iGlobalEvent = 0;
    return 0;
  }
  
  int iEventLowerBound  = 0;
  int iEventUpperBound  = _fileHeader.iNumIndex-1;
  int iEventTest        = (iEventLowerBound + iEventUpperBound)/2;

  // check special case: lower bound and upper bound
  {
    uint32_t uSecondsTest, uNanosecondsTest;
    
    int iError = time(iEventLowerBound, uSecondsTest, uNanosecondsTest);
    if (iError != 0)
    {
      printf("IndexFileReader::gotoTime(): Cannot get time for global event# %d\n", iEventLowerBound);
      return 2;
    }
    
    int iCompare = compareTime(uSeconds, uNanoseconds, uSecondsTest, uNanosecondsTest);
    if (iCompare <= 0)
    {
      iEventUpperBound = iEventLowerBound; // will pass the for loop below, and output iEventLowerBound as the index            
      if ( iCompare == 0 )
        bExactMatch = true;
    }
    else
    {

      iError = time(iEventUpperBound, uSecondsTest, uNanosecondsTest);
      if (iError != 0)
      {
        printf("IndexFileReader::gotoTime(): Cannot get time for global event# %d\n", iEventUpperBound);
        return 3;
      }
      
      iCompare = compareTime(uSeconds, uNanoseconds, uSecondsTest, uNanosecondsTest);
      if (iCompare > 0)
      {
        bOvertime = true;
        return 4; 
      }
        
      if (iCompare == 0)
      {
        iEventLowerBound = iEventUpperBound; // will pass the for loop below, and output iEventUpperBound as the index
        bExactMatch      = true;
      }
    }
  }
  
  /*
   * Use binary search to find the event with the correct time (the same or later time)
   */
  for (;iEventLowerBound+1 < iEventUpperBound; iEventTest = (iEventLowerBound + iEventUpperBound)/2)
  {
    uint32_t uSecondsTest, uNanosecondsTest;
    
    int iError = time(iEventTest, uSecondsTest, uNanosecondsTest);
    if (iError != 0)
    {
      printf("IndexFileReader::gotoTime(): Cannot get time for global event# %d\n", iEventTest);
      return 4;
    }
    
    int iCompare = compareTime(uSeconds, uNanoseconds, uSecondsTest, uNanosecondsTest);
    if (iCompare > 0)
      iEventLowerBound = iEventTest;
    else if (iCompare < 0)      
      iEventUpperBound = iEventTest;
    else
    {
      iEventUpperBound = iEventTest;
      bExactMatch      = true;
      break;
    }
  }
  
  iGlobalEvent = iEventUpperBound;  
  return 0;
}

int IndexFileReader::eventTimeToLocal(uint32_t uSeconds, uint32_t uNanoseconds, int& iCalib, int& iEvent, bool& bExactMatch, bool& bOvertime)
{
  iCalib      = -1;
  iEvent      = -1;
  bExactMatch = false;
  bOvertime   = false;
  
  int iGlobalEvent = -1;
  int iError = eventTimeToGlobal( uSeconds, uNanoseconds, iGlobalEvent, bExactMatch, bOvertime );
  if (iError != 0)
    return 1;
    
  iError = eventGlobalToLocal( iGlobalEvent, iCalib, iEvent );
  if (iError != 0)
    return 2;
  
  return 0;
}

int IndexFileReader::detectorList(int& iNumDetector, const ProcInfo*& lDetector) const
{
  iNumDetector = -1;
  if (_fXtcIndex == NULL)
    return 1;
    
  iNumDetector = _fileHeader.iNumDetector;
  lDetector    = &_lDetector[0];

  return 0;
}

int IndexFileReader::srcList(int iDetector, int& iNumSrc, const Src*& lSrc) const
{
  if (_fXtcIndex == NULL)
    return 1;
    
  if (iDetector < 0 || iDetector >= _fileHeader.iNumDetector )
  {
    printf("IndexFileReader::typeList(): Invalid detector id %d (max detId %d)\n",
      iDetector, _fileHeader.iNumDetector-1);
    return 2;
  }
  
  iNumSrc = _lSrcList[iDetector].size();
  lSrc    = &_lSrcList[iDetector][0];

  return 0;
}

int IndexFileReader::typeList(int iDetector, int& iNumType, const TypeId*& lType) const
{
  if (_fXtcIndex == NULL)
    return 1;

  if (iDetector < 0 || iDetector >= _fileHeader.iNumDetector )
  {
    printf("IndexFileReader::typeList(): Invalid detector id %d (max detId %d)\n",
      iDetector, _fileHeader.iNumDetector-1);
    return 2;
  }
  
  iNumType = _lTypeList[iDetector].size();
  lType = &_lTypeList[iDetector][0];

  return 0;
}

int IndexFileReader::calibCycleList(int& iNumCalib, const CalibNode*& lCalib) const
{
  iNumCalib = -1;
  if (_fXtcIndex == NULL)
    return 1;
    
  iNumCalib = _fileHeader.iNumCalib;
  lCalib    = &_lCalib[0];

  return 0;
}

int IndexFileReader::gotoEvent(int iCalib, int iEvent, int fdXtc, int& iGlobalEvent)
{  
  if (_fXtcIndex == NULL)
    return 1;
    
  iGlobalEvent = -1;
  if ( iCalib < 0 || iCalib >= (int) _lCalib.size() )
  {
    printf( "Invalid Calib Cycle %d\n", iCalib );
    return 2;
  }
     
  if ( iEvent < 0 )
  {
    const CalibNode& calibNode = _lCalib[iCalib];
    long long int lliOffsetSeek = lseek64(fdXtc, calibNode.lliOffset, SEEK_SET);
    if (lliOffsetSeek != calibNode.lliOffset)
    {
      printf( "IndexFileReader::gotoEvent(): seek xtc failed (expected 0x%Lx, get 0x%Lx), error = %s\n",
        calibNode.lliOffset, lliOffsetSeek, strerror(errno) );
      return 3;
    }   
    int iError = gotoL1Node(calibNode.iL1Index);
    if ( iError != 0 )
    {
      printf("IndexFileReader::gotoEvent(): cannot move index to L1 event 0x%x (%d)\n",
        calibNode.iL1Index, calibNode.iL1Index);
      return 4;
    }  
    
    iGlobalEvent = calibNode.iL1Index;    
    return 0;
  } 
  
  int iEventNumMax;
  numL1EventInCalib(iCalib, iEventNumMax);

  if ( iEvent >= iEventNumMax )
  {
    printf("IndexFileReader::gotoEvent(): L1 event %d (0x%x) is not in current calib cycle. max event# = %d (0x%x)\n",
      iEvent, iEvent, iEventNumMax-1, iEventNumMax-1);
    return 5;
  }
      
  int iEventFinal = _lCalib[iCalib].iL1Index + iEvent;    
  int iError = gotoL1Node(iEventFinal);
  if ( iError != 0 )
  {
    printf("IndexFileReader::gotoEvent(): cannot move index to L1 event 0x%x (%d)\n",
      iEventFinal, iEventFinal);
    return 6;
  }        
  
  long long int lliOffsetSeek = lseek64(fdXtc, _curL1Node.lliOffsetXtc, SEEK_SET);
  if (lliOffsetSeek != _curL1Node.lliOffsetXtc)
  {
    printf( "IndexFileReader::gotoEvent(): seek xtc failed (expected 0x%Lx, get 0x%Lx), error = %s\n",
      _curL1Node.lliOffsetXtc, lliOffsetSeek, strerror(errno) );
    return 7;
  }    
  
  iGlobalEvent = iEventFinal; 
  return 0;
}

int IndexFileReader::gotoTime(uint32_t uSeconds, uint32_t uNanoseconds, int fdXtc, int& iGlobalEvent, bool& bExactMatch, bool& bOvertime)
{  
  iGlobalEvent = -1;

  int iError = eventTimeToGlobal(uSeconds, uNanoseconds, iGlobalEvent, bExactMatch, bOvertime);
  if (iError != 0)
    return 1;
    
  iError = gotoL1Node(iGlobalEvent);
  if (iError != 0)
  {
    printf("IndexFileReader::gotoTime(): cannot move index to L1 event 0x%x (%d)\n",
      iGlobalEvent, iGlobalEvent);
    return 2;
  }        
  
  long long int lliOffsetSeek = lseek64(fdXtc, _curL1Node.lliOffsetXtc, SEEK_SET);
  if (lliOffsetSeek != _curL1Node.lliOffsetXtc)
  {
    printf( "IndexFileReader::gotoEvent(): seek xtc failed (expected 0x%Lx, get 0x%Lx), error = %s\n",
      _curL1Node.lliOffsetXtc, lliOffsetSeek, strerror(errno) );
    return 3;
  }    
  
  return 0;
}

int IndexFileReader::time(int iEvent, uint32_t& uSeconds, uint32_t& uNanoseconds)
{
  uSeconds      = 0;
  uNanoseconds  = 0;
  int iError = gotoL1Node(iEvent);
  if ( iError != 0 )
    return 1;
    
   uSeconds     = _curL1Node.uSeconds;  
   uNanoseconds = _curL1Node.uNanoseconds;
   return 0;
}

int IndexFileReader::fiducial(int iEvent, uint32_t& uFiducial)
{
  uFiducial = 0x1ffff;
  int iError = gotoL1Node(iEvent);
  if ( iError != 0 )
    return 1;
    
  uFiducial = _curL1Node.uFiducial;  
  return 0;
}

int IndexFileReader::damage(int iEvent, Damage& damage)
{
  int iError = gotoL1Node(iEvent);
  if ( iError != 0 )
    return 1;
    
  damage = _curL1Node.damage;
  return 0;
}

int IndexFileReader::detDmgMask(int iEvent, uint32_t& uMaskDetDmgs)
{
  int iError = gotoL1Node(iEvent);
  if ( iError != 0 )
    return 1;
    
  uMaskDetDmgs = _curL1Node.uMaskDetDmgs;
  return 0;
}

int IndexFileReader::detDataMask(int iEvent, uint32_t& uMaskDetData)
{
  int iError = gotoL1Node(iEvent);
  if ( iError != 0 )
    return 1;
    
  uMaskDetData = _curL1Node.uMaskDetData;
  return 0;
}

int IndexFileReader::evrEventList(int iEvent, unsigned int& uNumEvent, const uint8_t*& lEvrEvent)
{
  int iError = gotoL1Node(iEvent);
  if ( iError != 0 )
    return 1;
          
  uNumEvent = _lCurEvrEvent.size();
  lEvrEvent = &_lCurEvrEvent[0];
  return 0;  
}

int IndexFileReader::gotoL1Node(int iL1Node)
{
  if ( iL1Node == _iCurL1Node )
    return 0;
        
  if (_fXtcIndex == NULL)
    return 1;
    
  if ( iL1Node < 0 || iL1Node >= _fileHeader.iNumIndex )
  {
    printf( "IndexFileReader::gotoL1Node(): Invalid L1 event %d\n",
      iL1Node );
    return 2;
  }
      
  int iError = fseek(_fXtcIndex, _iHeaderSize + sizeof(_curL1Node)* iL1Node, SEEK_SET);
  if ( iError != 0 )
  {
    printf( "IndexFileReader::gotoL1Node(): seek to main data failed, error = %s\n",
      strerror(errno) );
    return 3;
  }  
  
  _iCurL1Node = -1;  
  _lCurEvrEvent.clear();
  
  int iItemsRead = fread( (void*)&_curL1Node, sizeof(_curL1Node), 1, _fXtcIndex);
  if ( iItemsRead != 1 )
  {
    printf( "IndexFileReader::gotoL1Node(): read L1 Node failed, error = %s\n",
      strerror(errno) );
    return 4;
  }
  
  /*
   * Generate current Evr events
   */
  if (_curL1Node.uMaskEvrEvents != 0)
  {
    uint32_t uBit = 0x1;
    for (int iBitIndex = 0; iBitIndex < (int)_lEvrEvent.size(); iBitIndex++, uBit <<= 1 )
      if ((_curL1Node.uMaskEvrEvents & uBit) != 0)
        _lCurEvrEvent.push_back(_lEvrEvent[iBitIndex]);
  }
        
  _iCurL1Node = iL1Node;  
  
  return 0;  
}

static int compareTime(uint32_t& uSeconds1, uint32_t& uNanoseconds1, uint32_t& uSeconds2, uint32_t& uNanoseconds2)
{
  if (uSeconds1 > uSeconds2) return 1;
  if (uSeconds1 < uSeconds2) return -1;  
  
  // remaining case: (uSeconds1 == uSeconds2)
  if (uNanoseconds1 > uNanoseconds2) return 1;
  if (uNanoseconds1 < uNanoseconds2) return -1;  
  return 0;  
}

} // namespace Index
} // namespace Pds
