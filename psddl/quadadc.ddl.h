#ifndef PDS_QUADADC_DDL_H
#define PDS_QUADADC_DDL_H 1

// *** Do not edit this file, it is auto-generated ***

#include <vector>
#include <iosfwd>
#include <cstddef>
#include <cstring>
#include "pdsdata/xtc/TypeId.hh"
#include "ndarray/ndarray.h"
namespace Pds {
namespace QuadAdc {

/** @class ConfigV1

  QuadAdc Class
*/

#pragma pack(push,4)

class ConfigV1 {
public:
  enum { TypeId = Pds::TypeId::Id_QuadAdcConfig /**< XTC type ID value (from Pds::TypeId class) */ };
  enum { Version = 0 /**< XTC type version number */ };
  ConfigV1(uint32_t arg__chanMask, double arg__delayTime, uint32_t arg__interleaveMode, uint32_t arg__nbrSamples, uint32_t arg__evtCode, double arg__sampRate)
    : _chanMask(arg__chanMask), _delayTime(arg__delayTime), _interleaveMode(arg__interleaveMode), _nbrSamples(arg__nbrSamples), _evtCode(arg__evtCode), _sampRate(arg__sampRate)
  {
  }
  ConfigV1() {}
  ConfigV1(const ConfigV1& other) {
    const char* src = reinterpret_cast<const char*>(&other);
    std::copy(src, src+other._sizeof(), reinterpret_cast<char*>(this));
  }
  ConfigV1& operator=(const ConfigV1& other) {
    const char* src = reinterpret_cast<const char*>(&other);
    std::copy(src, src+other._sizeof(), reinterpret_cast<char*>(this));
    return *this;
  }
  /** Channel Mask. */
  uint32_t chanMask() const { return _chanMask; }
  /** Delay time. */
  double delayTime() const { return _delayTime; }
  /** Interleave Mode. */
  uint32_t interleaveMode() const { return _interleaveMode; }
  /** Number of samples. */
  uint32_t nbrSamples() const { return _nbrSamples; }
  /** Event Code. */
  uint32_t evtCode() const { return _evtCode; }
  /** Sample Rate. */
  double sampleRate() const { return _sampRate; }
  static uint32_t _sizeof() { return 32; }
private:
  uint32_t	_chanMask;	/**< Channel Mask. */
  double	_delayTime;	/**< Delay time. */
  uint32_t	_interleaveMode;	/**< Interleave Mode. */
  uint32_t	_nbrSamples;	/**< Number of samples. */
  uint32_t	_evtCode;	/**< Event Code. */
  double	_sampRate;	/**< Sample Rate. */
};
#pragma pack(pop)
} // namespace QuadAdc
} // namespace Pds
#endif // PDS_QUADADC_DDL_H
