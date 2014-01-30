
// *** Do not edit this file, it is auto-generated ***

#include "pdsdata/psddl/partition.ddl.h"

#include <iostream>
namespace Pds {
namespace Partition {
Source::Source(const Pds::Src& arg__src, uint32_t arg__group)
    : _src(arg__src), _group(arg__group)
{
}
ConfigV1::ConfigV1(uint64_t arg__bldMask, uint32_t arg__numSources, const Partition::Source* arg__sources)
    : _bldMask(arg__bldMask), _numSources(arg__numSources)
{
  if (arg__sources and (this->numSources())) {
    ptrdiff_t offset = 12;
    Partition::Source* data = reinterpret_cast<Partition::Source*>(((char*)this)+offset);
    std::copy(arg__sources, arg__sources+(this->numSources()), data);
  }
}
} // namespace Partition
} // namespace Pds
