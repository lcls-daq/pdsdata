#ifndef Pds_SrcAlias_hh
#define Pds_SrcAlias_hh

#include "pdsdata/xtc/Src.hh"
#include "pdsdata/xtc/Level.hh"

namespace Pds {

  class SrcAlias:public Src {
  public:
    enum { AliasNameMax=30 };

    SrcAlias() {}
    SrcAlias(const Src&, const char* = 0);

    const char *  aliasName()     const;
  public:
    bool operator<(const SrcAlias&) const;
  protected:
    char          _aliasName[AliasNameMax];
    unsigned char _pad1;
    unsigned char _pad2;
  };

}
#endif
