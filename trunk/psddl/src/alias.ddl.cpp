
// *** Do not edit this file, it is auto-generated ***

#include "pdsdata/psddl/alias.ddl.h"

#include <iostream>
namespace Pds {
namespace Alias {
SrcAlias::SrcAlias(const Pds::Src& arg__src, const char* arg__aliasName)
    : _src(arg__src), _pad1(0)
{
  if (arg__aliasName) std::copy(arg__aliasName, arg__aliasName+(31), &_aliasName[0]);
}
uint8_t
SrcAlias::operator <(const Alias::SrcAlias& other) const {
  return (strcmp(this->_aliasName,other._aliasName)<0);
}
uint8_t
SrcAlias::operator ==(const Alias::SrcAlias& other) const {
  return (strcmp(this->_aliasName,other._aliasName)==0);
}
std::vector<int>
SrcAlias::aliasName_shape() const {
  std::vector<int> shape;
  shape.reserve(1);
  shape.push_back(AliasNameMax);
  return shape;
}
ConfigV1::ConfigV1(uint32_t arg__numSrcAlias, const Alias::SrcAlias* arg__srcAlias)
    : _numSrcAlias(arg__numSrcAlias)
{
  if (arg__srcAlias and (this->numSrcAlias())) {
    ptrdiff_t offset = 4;
    Alias::SrcAlias* data = reinterpret_cast<Alias::SrcAlias*>(((char*)this)+offset);
    std::copy(arg__srcAlias, arg__srcAlias+(this->numSrcAlias()), data);
  }
}
} // namespace Alias
} // namespace Pds
