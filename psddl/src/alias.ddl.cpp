
// *** Do not edit this file, it is auto-generated ***

#include "pdsdata/psddl/alias.ddl.h"

#include <iostream>
namespace Pds {
namespace Alias {
ConfigV1::ConfigV1(uint32_t arg__numSrcAlias, const Pds::SrcAlias* arg__srcAlias)
    : _numSrcAlias(arg__numSrcAlias)
{
  if (arg__srcAlias and (this->numSrcAlias())) {
    ptrdiff_t offset = 4;
    Pds::SrcAlias* data = reinterpret_cast<Pds::SrcAlias*>(((char*)this)+offset);
    std::copy(arg__srcAlias, arg__srcAlias+(this->numSrcAlias()), data);
  }
}
} // namespace Alias
} // namespace Pds
