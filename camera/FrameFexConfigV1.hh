#ifndef Camera_FrameFexConfigV1_hh
#define Camera_FrameFexConfigV1_hh

#include "pdsdata/camera/FrameCoord.hh"

namespace Camera {

  class FrameFexConfigV1 {
  public:
    enum Forwarding { NoFrame, FullFrame, RegionOfInterest };
    enum Processing { NoProcessing, GssFullFrame, GssRegionOfInterest, GssThreshold };

    FrameFexConfigV1();
    FrameFexConfigV1( Forwarding        forwarding,
		      unsigned          fwd_prescale,
		      Processing        processing,
		      const FrameCoord& roiBegin,
		      const FrameCoord& roiEnd,
		      unsigned          threshold,
		      unsigned          masked_pixels,
		      const FrameCoord* masked_coords );

    FrameFexConfigV1(const FrameFexConfigV1&);

    Forwarding        forwarding() const;
    unsigned          forward_prescale() const;
    Processing        processing() const;
    const FrameCoord& roiBegin  () const;
    const FrameCoord& roiEnd    () const;
    unsigned          threshold () const;
    unsigned          number_of_masked_pixels () const;
    const FrameCoord& masked_pixel_coordinates() const;

    unsigned size() const;

  private:
    uint32_t   _forwarding;
    uint32_t   _forward_prescale;
    uint32_t   _processing;
    FrameCoord _roiBegin;    
    FrameCoord _roiEnd;
    uint32_t   _threshold;
    uint32_t   _masked_pixel_count;
  };

};

#endif
