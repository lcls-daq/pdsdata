#ifndef Pds_Cspad_ElementIterator_hh
#define Pds_Cspad_ElementIterator_hh

//
//  class ElementIterator
//
//  A class to iterate through the CSPAD detector data
//
//  Each "Element" represents one quadrant of a complete detector
//  and they are arranged as follows (viewed from upstream):
//  +---+---+
//  | 0 | 1 |
//  +---+---+
//  | 3 | 2 |
//  +---+---+
//
//  Each "Element" is composed of 8 "Section"s arranged as follows:
//  +---+---+-------+
//  |   |   |   6   |
//  + 5 | 4 +-------+
//  |   |   |   7   |
//  +---+---+---+---+   (for quadrant 0)
//  |   2   |   |   |
//  +-------+ 0 | 1 |
//  |   3   |   |   |
//  +-------+---+---+
//  The layout of each successive quadrant is rotated 90 degrees clockwise
//  with respect to the previous quadrant.
//
//  Each "Section" is composed of 2*194 rows by 185 columns with the following 
//  orientations (for quadrant 0):
//    Sections 0,1: row index increases from bottom to top, column index increases from left to right
//    Sections 2,3: row index increases from left to right, column index increases from top to bottom
//    Sections 4,5: row index increases from top to bottom, column index increases from left to right
//    Sections 6,7: row index increases from left to right, column index increases from top to bottom
//  Again, the orientations of the Sections for quadrant 1 are rotated 90 degrees clockwise 
//  and so on for each successive quadrant.
//

#include "Detector.hh"

#include <stdint.h>

namespace Pds {
  namespace CsPad {

    class ConfigV1;
    class ConfigV2;
    class ElementV1;
    class ElementV2;

    class Section {
    public:
      uint16_t pixel[ColumnsPerASIC][2*MaxRowsPerASIC];
    };

    class ElementIterator {
    public:
      ElementIterator(const ConfigV1&, const ElementV1&);
      ElementIterator(const ConfigV2&, const ElementV1&);
      ElementIterator(const ConfigV2&, const ElementV2&);
    public:
      //  Iterate to the next Element/quadrant
      const ElementV1* next();
      const ElementV2* next();
      //  Iterate to the next Section within the current quadrant
      const Section* next(int sectionID&);
    private:
      const ElementV1* _elem1;
      const ElementV2* _elem2;
      unsigned         _qmask;
      unsigned         _amask;
      unsigned         _smask[4];
    };
  };
};

#endif
