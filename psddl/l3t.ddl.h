#ifndef PDS_L3T_DDL_H
#define PDS_L3T_DDL_H 1

// *** Do not edit this file, it is auto-generated ***

#include <vector>
#include <iosfwd>
#include <cstddef>
#include <cstring>
#include "pdsdata/xtc/TypeId.hh"
#include "ndarray/ndarray.h"
namespace Pds {
namespace L3T {

/** @class ConfigV1

  
*/

#pragma pack(push,4)

class ConfigV1 {
public:
  enum { TypeId = Pds::TypeId::Id_L3TConfig /**< XTC type ID value (from Pds::TypeId class) */ };
  enum { Version = 1 /**< XTC type version number */ };
  ConfigV1(uint32_t arg__module_id_len, uint32_t arg__desc_len, const char* arg__module_id, const char* arg__desc);
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
  /** Length of the module identification string */
  uint32_t module_id_len() const { return _module_id_len; }
  /** Length of the description string */
  uint32_t desc_len() const { return _desc_len; }
  /** The module identification string */
  const char* module_id() const { typedef char atype;
  ptrdiff_t offset=8;
  const atype* pchar = (const atype*)(((const char*)this)+offset);
  return pchar; }
  /** The description string */
  const char* desc() const { typedef char atype;
  ptrdiff_t offset=8+(1*(this->module_id_len()));
  const atype* pchar = (const atype*)(((const char*)this)+offset);
  return pchar; }
  uint32_t _sizeof() const { return (((((8+(1*(this->module_id_len())))+(1*(this->desc_len())))+4)-1)/4)*4; }
  /** Method which returns the shape (dimensions) of the data returned by module_id() method. */
  std::vector<int> module_id_shape() const;
  /** Method which returns the shape (dimensions) of the data returned by desc() method. */
  std::vector<int> desc_shape() const;
private:
  uint32_t	_module_id_len;	/**< Length of the module identification string */
  uint32_t	_desc_len;	/**< Length of the description string */
  //char	_module_id[this->module_id_len()];
  //char	_desc[this->desc_len()];
};
#pragma pack(pop)

/** @class DataV1

  
*/

#pragma pack(push,4)

class DataV1 {
public:
  enum { TypeId = Pds::TypeId::Id_L3TData /**< XTC type ID value (from Pds::TypeId class) */ };
  enum { Version = 1 /**< XTC type version number */ };
  DataV1(uint32_t arg__accept)
    : _accept(arg__accept)
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
  /** Module trigger decision */
  uint32_t accept() const { return _accept; }
  static uint32_t _sizeof() { return 4; }
private:
  uint32_t	_accept;	/**< Module trigger decision */
};
#pragma pack(pop)
} // namespace L3T
} // namespace Pds
#endif // PDS_L3T_DDL_H
