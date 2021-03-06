
// *** Do not edit this file, it is auto-generated ***

#include "pdsdata/psddl/l3t.ddl.h"

#include <iostream>
namespace Pds {
namespace L3T {
ConfigV1::ConfigV1(uint32_t arg__module_id_len, uint32_t arg__desc_len, const char* arg__module_id, const char* arg__desc)
    : _module_id_len(arg__module_id_len), _desc_len(arg__desc_len)
{
  if (arg__module_id and (this->module_id_len())) {
    ptrdiff_t offset = 8;
    char* data = reinterpret_cast<char*>(((char*)this)+offset);
    std::copy(arg__module_id, arg__module_id+(this->module_id_len()), data);
  }
  if (arg__desc and (this->desc_len())) {
    ptrdiff_t offset = 8+(1*(this->module_id_len()));
    char* data = reinterpret_cast<char*>(((char*)this)+offset);
    std::copy(arg__desc, arg__desc+(this->desc_len()), data);
  }
}
std::vector<int>
ConfigV1::module_id_shape() const {
  std::vector<int> shape;
  shape.reserve(1);
  shape.push_back(this->module_id_len());
  return shape;
}
std::vector<int>
ConfigV1::desc_shape() const {
  std::vector<int> shape;
  shape.reserve(1);
  shape.push_back(this->desc_len());
  return shape;
}
std::ostream& operator<<(std::ostream& str, L3T::DataV2::Result enval) {
  const char* val;
  switch (enval) {
  case L3T::DataV2::Fail:
    val = "Fail";
    break;
  case L3T::DataV2::Pass:
    val = "Pass";
    break;
  case L3T::DataV2::None:
    val = "None";
    break;
  default:
    return str << "Result(" << int(enval) << ")";
  }
  return str << val;
}
std::ostream& operator<<(std::ostream& str, L3T::DataV2::Bias enval) {
  const char* val;
  switch (enval) {
  case L3T::DataV2::Unbiased:
    val = "Unbiased";
    break;
  case L3T::DataV2::Biased:
    val = "Biased";
    break;
  default:
    return str << "Bias(" << int(enval) << ")";
  }
  return str << val;
}
} // namespace L3T
} // namespace Pds
