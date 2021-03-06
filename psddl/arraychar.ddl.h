#ifndef PDS_ARRAYCHAR_DDL_H
#define PDS_ARRAYCHAR_DDL_H 1

// *** Do not edit this file, it is auto-generated ***

#include <vector>
#include <iosfwd>
#include <cstddef>
#include <cstring>
#include "pdsdata/xtc/TypeId.hh"
#include "ndarray/ndarray.h"
namespace Pds {
namespace Arraychar {

/** @class DataV1

  
*/

#pragma pack(push,4)

class DataV1 {
public:
  enum { TypeId = Pds::TypeId::Id_Arraychar /**< XTC type ID value (from Pds::TypeId class) */ };
  enum { Version = 1 /**< XTC type version number */ };
  DataV1(uint64_t iNumChars)
    : _iNumChars(iNumChars)
  {
  }
  DataV1() {}
  DataV1(const DataV1& other) {
    const char* src = reinterpret_cast<const char*>(&other);
    std::copy(src, src+other._sizeof(), reinterpret_cast<char*>(this));
  }
  DataV1& operator=(const DataV1& other) {
    const char* src = reinterpret_cast<const char*>(&other);
    std::copy(src, src+other._sizeof(), reinterpret_cast<char*>(this));
    return *this;
  }
  uint64_t numChars() const { return _iNumChars; }
  /**     Note: this overloaded method accepts shared pointer argument which must point to an object containing
    this instance, the returned ndarray object can be used even after this instance disappears. */
  template <typename T>
  ndarray<const uint8_t, 1> data(const boost::shared_ptr<T>& owner) const { 
    ptrdiff_t offset=8;
    const uint8_t* data = (const uint8_t*)(((char*)this)+offset);
    return make_ndarray(boost::shared_ptr<const uint8_t>(owner, data), this->_iNumChars);
  }
  /**     Note: this method returns ndarray instance which does not control lifetime
    of the data, do not use returned ndarray after this instance disappears. */
  ndarray<const uint8_t, 1> data() const { ptrdiff_t offset=8;
  const uint8_t* data = (const uint8_t*)(((char*)this)+offset);
  return make_ndarray(data, this->_iNumChars); }
  uint32_t _sizeof() const { return ((((8+(1*(this->_iNumChars)))+4)-1)/4)*4; }
private:
  uint64_t	_iNumChars;
  //uint8_t	_data[this->_iNumChars];
};
#pragma pack(pop)
} // namespace Arraychar
} // namespace Pds
#endif // PDS_ARRAYCHAR_DDL_H
