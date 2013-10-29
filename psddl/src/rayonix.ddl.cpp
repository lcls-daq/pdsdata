
// *** Do not edit this file, it is auto-generated ***

#include "pdsdata/psddl/rayonix.ddl.h"

#include <iostream>
namespace Pds {
namespace Rayonix {
std::ostream& operator<<(std::ostream& str, Rayonix::ConfigV1::ReadoutMode enval) {
  const char* val;
  switch (enval) {
  case Rayonix::ConfigV1::Standard:
    val = "Standard";
    break;
  case Rayonix::ConfigV1::HighGain:
    val = "HighGain";
    break;
  case Rayonix::ConfigV1::LowNoise:
    val = "LowNoise";
    break;
  case Rayonix::ConfigV1::EDR:
    val = "EDR";
    break;
  default:
    return str << "ReadoutMode(" << int(enval) << ")";
  }
  return str << val;
}
} // namespace Rayonix
} // namespace Pds
