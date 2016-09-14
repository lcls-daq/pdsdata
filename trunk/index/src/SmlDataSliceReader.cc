#include "pdsdata/index/SmlDataSliceReader.hh"

namespace Pds
{  
namespace SmlData
{

int SmlDataSliceReader::open(const char* sXtcIndex)
{
  return 0;
}

int SmlDataSliceReader::close()
{
  return 0;
}

bool SmlDataSliceReader::isValid() const 
{
  return 0;
}

int SmlDataSliceReader::numL1Event(int& iNumL1Event) const
{
  return 0;
}

int SmlDataSliceReader::detectorList(int& iNumDetector, const ProcInfo*& lDetector) const
{
  return 0;
}

int SmlDataSliceReader::srcList(int iDetector, int& iNumSrc, const Src*& lSrc) const
{
  return 0;
}

int SmlDataSliceReader::typeList(int iDetector, int& iNumType, const TypeId*& lType) const
{
  return 0;
}

int SmlDataSliceReader::calibCycleList(int& iNumCalib, const CalibNode*& lCalib) const
{
  return 0;
}

int SmlDataSliceReader::numL1EventInCalib(int iCalib, int& iNumL1Event) const
{
  return 0;
}

int SmlDataSliceReader::eventLocalToGlobal(int iCalib, int iEvent, int& iGlobalEvent) const
{
  return 0;
}

int SmlDataSliceReader::eventGlobalToLocal(int iGlobalEvent, int& iCalib, int& iEvent) const
{
  return 0;
}

int SmlDataSliceReader::eventTimeToGlobal(uint32_t uSeconds, uint32_t uNanoseconds, int& iGlobalEvent, bool& bExactMatch, bool& bOvertime)
{
  return 0;
}

int SmlDataSliceReader::eventTimeToLocal(uint32_t uSeconds, uint32_t uNanoseconds, int& iCalib, int& iEvent, bool& bExactMatch, bool& bOvertime)
{
  return 0;
}

int SmlDataSliceReader::gotoEvent(int iCalib, int iEvent, int64_t& i64Offset, int& iGlobalEvent)
{
  return 0;
}

int SmlDataSliceReader::gotoEventAndSeek(int iCalib, int iEvent, int fdXtc, int& iGlobalEvent)
{
  return 0;
}

int SmlDataSliceReader::gotoTime(uint32_t uSeconds, uint32_t uNanoseconds, int fdXtc, int& iGlobalEvent, bool& bExactMatch, bool& bOvertime)
{
  return 0;
}

int SmlDataSliceReader::time(int iEvent, uint32_t& uSeconds, uint32_t& uNanoseconds)
{
  return 0;
}

int SmlDataSliceReader::fiducial(int iEvent, uint32_t& uFiducial)
{
  return 0;
}

int SmlDataSliceReader::damage(int iEvent, Damage& damage)
{
  return 0;
}

int SmlDataSliceReader::detDmgMask(int iEvent, uint32_t& uMaskDetDmgs)
{
  return 0;
}

int SmlDataSliceReader::detDataMask(int iEvent, uint32_t& uMaskDetData)
{
  return 0;
}

int SmlDataSliceReader::evrEventList(int iEvent, unsigned int& uNumEvent, const uint8_t*& lEvrEvent)
{
  return 0;
}
  
}//namespace SmlData
}//namespace Pds
