/*
 * ElementHeader.hh
 *
 *  Created on: April 25, 2013
 *      Author: jackp
 */

#ifndef ELEMENTHEADER_HH_
#define ELEMENTHEADER_HH_

#include <stdint.h>
#include <stddef.h>

//   Header = 8 x 32-bits
//       Header[0] = Tid[23:0], Lane[1:0], Z[3:0], VC[1:0]
//       Header[1] = Z[3:0], Quad[3:0], OpCode[7:0], acqCount[15:0]
//       Header[2] = BeamCode[31:0]
//       Header[3] = Z[31:0]
//       Header[4] = Z[31:0]
//       Header[5] = Z[31:0]
//       Header[6] = DiodeCurrent[31:0] (inserted by software)
//       Header[7] = FrameType[31:0]

namespace Pds {
  namespace Imp {

//    class FirstWordBits {
//      public:
//        unsigned vc:       2;    // 1:0
//        unsigned z:        4;    // 5:2
//        unsigned lane:     2;    // 7:6
//        unsigned tid:     24;    //31:8
//    };
//
//    class SecondWordBits {
//      public:
//        unsigned z:       32;    //31:0
//    };
//
//    class LaneStatus {
//      public:
//        LaneStatus() {};
//        ~LaneStatus() {};
//      public:
//        unsigned usLinkErrCount:     4;  // (3:0)
//        unsigned usLinkDownCount:    4;  // (7:4)
//        unsigned usCellErrCount:     4;  // (11:8)
//        unsigned usRxCount:          4;  // (15:12)
//        unsigned usLocLinked:        1;  // (16)
//        unsigned usRemLinked:        1;  // (17)
//        unsigned zeros:             10;  // (27:18)
//        unsigned powersOkay:         4;  // (31:28)
//    };

    class ElementHeader {
      public:
        ElementHeader() {};
        ~ElementHeader() {};

      public:
        unsigned   vc()              { return (unsigned) first & 3; }
        unsigned   lane()            { return (unsigned) (first & 0xc0)>>6; }
        unsigned   frameNumber()     { return (unsigned) _frameNumber; }
        uint32_t   range()           { return _range; }
        const unsigned  vc()          const { return (unsigned) first & 3; }
        const unsigned  lane()        const { return (unsigned) (first & 0xc0)>>6; }
        const unsigned  frameNumber() const { return (unsigned) _frameNumber; }
        const uint32_t  range()       const { return _range; }
        const uint32_t  laneStatus()  const { return _lane; }

      private:
        uint32_t              first;         // 0
        uint32_t              second;        // 1
        uint32_t              _frameNumber;   // 2
        uint32_t              _ticks;
        uint32_t              _fiducials;
        uint32_t              _range;
        uint32_t              _lane;
        uint32_t              _z;     // 7
    };

  }
}

#endif /* ELEMENTHEADER_HH_ */
