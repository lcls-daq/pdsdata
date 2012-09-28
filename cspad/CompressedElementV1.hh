#ifndef Pds_CompressedElementV1_hh
#define Pds_CompressedElementV1_hh

#include "ElementHeader.hh"

#include "pdsdata/xtc/CompressedPayload.hh"

namespace Pds {
  namespace CsPad {
    class ElementV1;
    class CompressedElementV1 : public ElementHeader {
    public:
      CompressedElementV1();
      CompressedElementV1(const ElementV1&);
    public:
      const CompressedPayload& pd() const;
    private:
      CompressedPayload _pd;
    };

    ElementV1* uncompressed(const CompressedElementV1&);
  };
};
#endif
