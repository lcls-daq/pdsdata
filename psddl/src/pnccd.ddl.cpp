
// *** Do not edit this file, it is auto-generated ***

#include "pdsdata/psddl/pnccd.ddl.h"

#include <iostream>
namespace Pds {
namespace PNCCD {
std::vector<int>
ConfigV2::info_shape() const {
  std::vector<int> shape;
  shape.reserve(1);
  shape.push_back(256);
  return shape;
}
std::vector<int>
ConfigV2::timingFName_shape() const {
  std::vector<int> shape;
  shape.reserve(1);
  shape.push_back(256);
  return shape;
}
std::vector<int>
FramesV1::frame_shape(const PNCCD::ConfigV1& cfg) const {
  std::vector<int> shape;
  shape.reserve(1);
  shape.push_back(cfg.numLinks());
  return shape;
}
std::vector<int>
FramesV1::frame_shape(const PNCCD::ConfigV2& cfg) const {
  std::vector<int> shape;
  shape.reserve(1);
  shape.push_back(cfg.numLinks());
  return shape;
}
} // namespace PNCCD
} // namespace Pds
