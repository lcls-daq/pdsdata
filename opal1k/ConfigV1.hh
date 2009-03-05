#ifndef Opal1k_ConfigV1_hh
#define Opal1k_ConfigV1_hh

#include <stdint.h>

namespace Camera {
  class FrameCoord;
};

namespace Opal1k {

  class ConfigV1 {
  public:
    enum Depth     { Eight_bit, Ten_bit, Twelve_bit };
    enum Binning   { x1, x2, x4, x8 };
    enum Mirroring { None, HFlip, VFlip, HVFlip };
    enum { LUT_Size=4096 };

    ConfigV1();
    ConfigV1(unsigned short  black,
	     unsigned short  gain,
	     Depth           depth,
	     Binning         binning,
	     Mirroring       mirroring,
	     bool            vertical_remap,
	     bool            enable_pixel_correction );

    ConfigV1(unsigned short   black,
	     unsigned short   gain,
	     Depth            depth,
	     Binning          binning,
	     Mirroring        mirroring,
	     bool             vertical_remap,
	     bool             enable_pixel_correction,
	     const uint16_t*  lut);

    //  Accessors
    unsigned short   black_level() const;
    unsigned short   gain_percent() const;
    unsigned short   output_offset() const;
    Depth            output_resolution() const;
    unsigned         output_resolution_bits() const;
    Binning          vertical_binning() const;
    Mirroring        output_mirroring() const;
    bool             vertical_remapping() const;
    bool             defect_pixel_correction_enabled() const;

    bool             output_lookup_table_enabled() const;
    const
    uint16_t*        output_lookup_table() const;

    unsigned         number_of_defect_pixels() const;
    void             set_number_of_defect_pixels(unsigned);
    const
    Camera::FrameCoord* defect_pixel_coordinates() const;
    Camera::FrameCoord* defect_pixel_coordinates();

    unsigned         size() const;

  private:
    uint32_t _offsetAndGain;
    uint32_t _outputOptions; // bit mask of output formatting options
    uint32_t _defectPixelCount;
  };

};

#endif
