#include "pdsdata/princeton/ConfigV1.hh"

#include <string.h>

using namespace Pds;
using namespace Princeton;

ConfigV1::ConfigV1(uint32_t uWidth, uint32_t uHeight, uint32_t uOrgX, uint32_t uOrgY, uint32_t uBinX, uint32_t uBinY) :
 _uWidth(uWidth), _uHeight(uHeight), _uOrgX(uOrgX), _uOrgY(uOrgY), _uBinX(uBinX), _uBinY(uBinY)
 {}
