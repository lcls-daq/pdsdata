#ifndef Pds_CompressedElementV2_hh
#define Pds_CompressedElementV2_hh

#include "ElementHeader.hh"

#include "pdsdata/xtc/CompressedPayload.hh"

namespace Pds {
  namespace CsPad {
    class ElementV2;
    class CompressedElementV2 : public ElementHeader {
    public:
      CompressedElementV2();
      CompressedElementV2(const ElementV2&);
    public:
      const CompressedPayload& pd() const;
    private:
      CompressedPayload _pd;
    };

    ElementV2* uncompressed(const CompressedElementV2&);
  };
};
#endif
