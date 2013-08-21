#include "pdsdata/xtc/SrcAlias.hh"
#include <string.h>

using namespace Pds;

SrcAlias::SrcAlias(const Src& source, const char *aliasName)
   : Src(source)
{
  if (aliasName) {
    strncpy(_aliasName, aliasName, AliasNameMax);
  } else {
    _aliasName[0] = '\0';
  }
  _pad1 = 0;
}

bool SrcAlias::operator<(const SrcAlias& m) const
{
  int nt = strncmp(_aliasName, m._aliasName, AliasNameMax);
  return (nt<0);
}

const char *    SrcAlias::aliasName()     const {return (_aliasName);}

