#include "pdsdata/alias/ConfigV1.hh"
#include "pdsdata/xtc/DetInfo.hh"
#include <list>
#include <stdio.h>

using namespace Pds::Alias;

static unsigned int appendAliases(const std::list<Pds::SrcAlias>& aliases, ConfigV1* s, unsigned int limit)
{
  unsigned int count = 0;
  std::list<Pds::SrcAlias> sorted_aliases (aliases); sorted_aliases.sort();

  Pds::SrcAlias* aa = reinterpret_cast<Pds::SrcAlias*>(s+1);
  for (std::list<Pds::SrcAlias>::const_iterator iter = sorted_aliases.begin();
       (iter != sorted_aliases.end()); ++iter) {
    if (count >= limit) {
      printf("Error: limit of %u aliases exceeded in %s\n", limit, __FUNCTION__);
      // do not append aliases exceeding limit
      break;
    }
    *aa++ = *iter;
    ++count;
  }
  return (count);
}

ConfigV1::ConfigV1 (std::list<Pds::SrcAlias>& aliasList)
{
  _numSrcAlias = appendAliases(aliasList, this, ConfigV1::MaxAliasCount);
}

ConfigV1::ConfigV1 () :
  _numSrcAlias  (0)
{
}

Pds::SrcAlias *ConfigV1::srcAliasGet(unsigned int index) const
{
  Pds::SrcAlias *rv = 0;

  if (index < _numSrcAlias) {
    Pds::SrcAlias *tmp = (Pds::SrcAlias *)(this+1);
    rv = &tmp[index];
  }
  return (rv);
}

void ConfigV1::print() const
{
  SrcAlias *pAlias;
  DetInfo *pInfo;
  char buf[Pds::SrcAlias::AliasNameMax+1];

  printf("Number of Aliases: %u\n", _numSrcAlias);
  for (int ii = 0; ii < (int)_numSrcAlias; ii++) {
    pAlias = srcAliasGet(ii);
    if (pAlias) {
      pInfo = (DetInfo*)pAlias;
      strncpy(buf, pAlias->aliasName(), Pds::SrcAlias::AliasNameMax);
      buf[Pds::SrcAlias::AliasNameMax] = '\0';
      printf("[%02d] %s (%s,%d  %s,%d)\n", ii, buf,
             DetInfo::name(pInfo->detector()), pInfo->detId(),
             DetInfo::name(pInfo->device()), pInfo->devId());
    } else {
      printf("(null)\n");
    }
  }
}
