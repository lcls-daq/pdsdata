#include <errno.h>

#include "pdsdata/index/IndexFileReader.hh"

namespace Pds
{  
namespace Index
{

IndexFileReader::IndexFileReader() : _fXtcIndex(NULL), _iHeaderSize(0), _iCurL1Node(-1), _bExtDataRead(false)
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

  if (_fileHeader.iNumDetector > 0)
  {
    _lDetector.resize(_fileHeader.iNumDetector, ProcInfo(Level::Segment,0,0));
    iItemsRead = fread( (void*) &_lDetector[0], _lDetector.size() * sizeof(_lDetector[0]), 1, _fXtcIndex);
    if ( iItemsRead != 1 )
    {
      printf( "IndexFileReader::open(): read detector (%d) failed, error = %s\n",
        _fileHeader.iNumDetector, strerror(errno) );
      return 3;
    }  
  }

  if (_fileHeader.iNumCalib > 0)
  {
    _lCalib.resize(_fileHeader.iNumCalib);
    iItemsRead = fread( (void*) &_lCalib[0], _lCalib.size() * sizeof(_lCalib[0]), 1, _fXtcIndex);
    if ( iItemsRead != 1 )
    {
      printf( "IndexFileReader::open(): read calib cycle (%d) failed, error = %s\n",
        _fileHeader.iNumCalib, strerror(errno) );
      return 4;
    }  
  }
  
  _iHeaderSize = sizeof(_fileHeader) + 
    sizeof(ProcInfo)  * _fileHeader.iNumDetector + 
    sizeof(CalibNode) * _fileHeader.iNumCalib;
  
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
  _lDetector.clear();
  _lCalib   .clear();
  
  return 0;
}

bool IndexFileReader::isValid() const
{
  return (_fXtcIndex != NULL);
}

int IndexFileReader::numL1Event(int& uNumL1Event) const
{
  uNumL1Event = -1;
  if (_fXtcIndex == NULL)
    return 1;

  uNumL1Event = _fileHeader.iNumIndex;
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

int IndexFileReader::calibCyleList(int& iNumCalib, const CalibNode*& lCalib) const
{
  iNumCalib = -1;
  if (_fXtcIndex == NULL)
    return 1;
    
  iNumCalib = _fileHeader.iNumCalib;
  lCalib    = &_lCalib[0];

  return 0;
}

int IndexFileReader::gotoEvent(int iEventL1Accept, int fdXtc)
{
  int iError = gotoL1Node(iEventL1Accept);
  if ( iError != 0 )
    return 1;
      
  long long int lliOffsetSeek = lseek64(fdXtc, _curL1Node.lliOffsetXtc, SEEK_SET);
  if (lliOffsetSeek != _curL1Node.lliOffsetXtc)
  {
    printf( "IndexFileReader::gotoEvent(): seek xtc failed (expected 0x%Lx, get 0x%Lx), error = %s\n",
      _curL1Node.lliOffsetXtc, lliOffsetSeek, strerror(errno) );
    return 2;
  }    
  
  return 0;
}

int IndexFileReader::gotoCalibCycle(int iCalib, int fdXtc, int& iEventAfterSeek)
{
  iEventAfterSeek = -1;
  if ( iCalib < 0 || iCalib >= (int) _lCalib.size() )
  {
    printf( "Invalid Calib Cycle %d\n", iCalib );
    return 1;
  }
  
  const CalibNode& calibNode = _lCalib[iCalib];
  long long int lliOffsetSeek = lseek64(fdXtc, calibNode.lliOffset, SEEK_SET);
  if (lliOffsetSeek != calibNode.lliOffset)
  {
    printf( "IndexFileReader::gotoCalibCyle(): seek xtc failed (expected 0x%Lx, get 0x%Lx), error = %s\n",
      calibNode.lliOffset, lliOffsetSeek, strerror(errno) );
    return 2;
  }    
    
  int iError = gotoL1Node(calibNode.iL1Index);
  if ( iError != 0 )
  {
    printf("IndexFileReader::gotoCalibCyle(): cannot jump to L1 event 0x%x (%d)\n",
      calibNode.iL1Index, calibNode.iL1Index);
    return 3;
  }  
  iEventAfterSeek = calibNode.iL1Index;
  
  return 0;
  
}

int IndexFileReader::fiducial(int iEventL1Accept, uint32_t& uFiducial, bool& bLnkNext, bool& bLnkPrev)
{
  uFiducial = 0x1ffff;
  int iError = gotoL1Node(iEventL1Accept);
  if ( iError != 0 )
    return 1;
    
  uFiducial = _curL1Node.uFiducial;
  bLnkNext  = _curL1Node.linkToNext();
  bLnkPrev  = _curL1Node.linkToPrev();
  
  return 0;
}

int IndexFileReader::checkEpics(int iEventL1Accept, bool& bEpics)
{
  int iError = gotoL1Node(iEventL1Accept);
  if ( iError != 0 )
    return 1;
    
  bEpics = _curL1Node.hasEpics();  
  return 0;
}

int IndexFileReader::checkPrinceton(int iEventL1Accept, bool& bPrinceton)
{
  int iError = gotoL1Node(iEventL1Accept);
  if ( iError != 0 )
    return 1;
    
  bPrinceton = _curL1Node.hasPrinceton();  
  return 0;
}

int IndexFileReader::damageSummary(int iEventL1Accept, Damage& damage)
{
  int iError = gotoL1Node(iEventL1Accept);
  if ( iError != 0 )
    return 1;
    
  damage = _curL1Node.damage;
  return 0;
}

int IndexFileReader::detDmgMask(int iEventL1Accept, uint32_t& uDamageMask)
{
  int iError = gotoL1Node(iEventL1Accept);
  if ( iError != 0 )
    return 1;
    
  uDamageMask = _curL1Node.uDetDmgMask;
  return 0;
}


int IndexFileReader::evrEventList(int iEventL1Accept, unsigned int& uNumEvent, const uint8_t*& lEvrEvent)
{
  int iError = gotoL1Node(iEventL1Accept);
  if ( iError != 0 )
    return 1;
  
  readNodeExtData();
  if ( !_bExtDataRead )
    return 2;
  
  uNumEvent = _iCurNumEvrEvent;
  lEvrEvent = &_lCurEvrEvent[0];
  return 0;  
}

int IndexFileReader::damageList(int iEventL1Accept, unsigned int& uNumDamage, const SegDmg*& lDamage)
{
  int iError = gotoL1Node(iEventL1Accept);
  if ( iError != 0 )
    return 1;
  
  readNodeExtData();
  if ( !_bExtDataRead )
    return 2;
  
  uNumDamage  = _iCurNumDamage;
  lDamage     = &_lCurDamage[0];
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
  
  _iCurL1Node    = -1;
  _bExtDataRead = false;  
  
  int iItemsRead = fread( (void*)&_curL1Node, sizeof(_curL1Node), 1, _fXtcIndex);
  if ( iItemsRead != 1 )
  {
    printf( "IndexFileReader::gotoL1Node(): read L1 Node failed, error = %s\n",
      strerror(errno) );
    return 4;
  }
        
  _iCurL1Node = iL1Node;
  
  return 0;  
}

int IndexFileReader::readNodeExtData()
{
  if ( _bExtDataRead )
  return 0;

  if ( _fXtcIndex == NULL )
    return 1;
  if ( _iCurL1Node < 0 )
    return 2;
    
  /*
   * reset node Extlemental data
   */  
  _iCurNumEvrEvent  = -1;
  _iCurNumDamage    = -1;  
  _lCurEvrEvent .clear();
  _lCurDamage   .clear();
  
  
  int iError = fseek(_fXtcIndex, _curL1Node.uOffsetExt, SEEK_SET);
  if ( iError != 0 )
  {
    printf( "IndexFileReader::gotoL1Node(): seek to Ext data failed, error = %s\n",
      strerror(errno) );
    return 4;
  }  
  
  int iItemsRead = fread( (void*)&_iCurNumEvrEvent, sizeof(_iCurNumEvrEvent), 1, _fXtcIndex);
  if ( iItemsRead != 1 )
  {
    printf( "IndexFileReader::gotoL1Node(): read # evr events failed, error = %s\n",
      strerror(errno) );
    return 5;
  }  
  
  if (_iCurNumEvrEvent > 0)
  {
    _lCurEvrEvent.resize(_iCurNumEvrEvent);
    iItemsRead = fread( (void*) &_lCurEvrEvent[0], _lCurEvrEvent.size() * sizeof(_lCurEvrEvent[0]), 1, _fXtcIndex);
    if ( iItemsRead != 1 )
    {
      printf( "IndexFileReader::gotoL1Node(): read evr events (%d) failed, error = %s\n",
        _iCurNumEvrEvent, strerror(errno) );
      return 6;
    }  
  }
  
  iItemsRead = fread( (void*)&_iCurNumDamage, sizeof(_iCurNumDamage), 1, _fXtcIndex);
  if ( iItemsRead != 1 )
  {
    printf( "IndexFileReader::gotoL1Node(): read # damages failed, error = %s\n",
      strerror(errno) );
    return 5;
  }  
  
  if (_iCurNumDamage > 0)
  {
    _lCurDamage.resize(_iCurNumDamage);
    iItemsRead = fread( (void*) &_lCurDamage[0], _lCurDamage.size() * sizeof(_lCurDamage[0]), 1, _fXtcIndex);
    if ( iItemsRead != 1 )
    {
      printf( "IndexFileReader::gotoL1Node(): read damages failed, error = %s\n",
        strerror(errno) );
      return 6;
    }  
  }  
  
  _bExtDataRead = true;
  return 0;  
}

} // namespace Index
} // namespace Pds
