#ifndef ALIASCONFIGV1_HH
#define ALIASCONFIGV1_HH

#include <stdint.h>
#include <list>
#include "pdsdata/xtc/SrcAlias.hh"

#pragma pack(4)

namespace Pds {

  namespace Alias {

    /*
     * This class is an expanding class
     *
     * A SrcAlias array is attached to the end of this class. 
     * The length of the array is defined by _numSrcAlias.
     * The maximum length of the array is defined by MaxAliasCount.
     */
    class ConfigV1 {
    public:
      enum { Version = 1 };
      enum { MaxAliasCount = 256 };
      enum { MaxPayloadSize = MaxAliasCount * sizeof(SrcAlias) };

      ConfigV1();
      ConfigV1(std::list<Pds::SrcAlias>& aliases);

      unsigned int      srcAliasCount()                 const     { return _numSrcAlias; }
      Pds::SrcAlias *   srcAliasGet(unsigned int index) const;
      void              print()                         const;
      int               size()                          const     { return sizeof(*this) + _numSrcAlias * sizeof(SrcAlias); }

      static void* operator new(size_t size, char* p)     { return p; }
      // Disable ordinary (non-placement) new: only placement new and memory mapped objects are allowed
      // Disable placement delete: Not allow to delete memory mapped objects

    private:
      unsigned int      _numSrcAlias;
    };
  };
};

#pragma pack()

#endif
