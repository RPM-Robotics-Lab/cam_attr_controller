#include "bluefox2_setting.h"
#include "lcmtypes++/bot_core/image_t.hpp"

using namespace std;

namespace bluefox2 {


int PixelFormatToEncoding(const TImageBufferPixelFormat& pixel_format) 
{
    switch (pixel_format) {
    case ibpfRaw:
        // return bot_core::image_t::PIXEL_FORMAT_GRAY;
    case ibpfMono8:
        return bot_core::image_t::PIXEL_FORMAT_GRAY;
    case ibpfMono16:
        return bot_core::image_t::PIXEL_FORMAT_BE_GRAY16; // Big Endian
        // return bot_core::image_t::PIXEL_FORMAT_LE_GRAY16; // Little Endian
    case ibpfRGBx888Packed:
        return bot_core::image_t::PIXEL_FORMAT_BGRA;
    case ibpfYUV422Packed:
    case ibpfRGBx888Planar:
    case ibpfMono10:
    case ibpfMono12:
    case ibpfMono14:
    case ibpfRGB888Packed:
        return bot_core::image_t::PIXEL_FORMAT_BGR;
    case ibpfYUV444Planar:
    case ibpfMono32:
    case ibpfYUV422Planar:
    case ibpfRGB101010Packed:
    case ibpfRGB121212Packed:
    case ibpfRGB141414Packed:
    case ibpfRGB161616Packed:
        return bot_core::image_t::PIXEL_FORMAT_BE_RGB16;
        // return bot_core::image_t::PIXEL_FORMAT_LE_RGB16;
    case ibpfYUV422_UYVYPacked:
    case ibpfMono12Packed_V2:
    case ibpfYUV422_10Packed:
    case ibpfYUV422_UYVY_10Packed:
    case ibpfBGR888Packed:
        return bot_core::image_t::PIXEL_FORMAT_RGB;
    case ibpfBGR101010Packed_V2:
    case ibpfYUV444_UYVPacked:
    case ibpfYUV444_UYV_10Packed:
    case ibpfYUV444Packed:
    case ibpfYUV444_10Packed:
    case ibpfMono12Packed_V1:
    case ibpfYUV411_UYYVYY_Packed:
    case ibpfRGB888Planar:        
    default:
        return bot_core::image_t::PIXEL_FORMAT_GRAY;
    }
}

int BayerPatternToEncoding(const TBayerMosaicParity& bayer_pattern,
                                   int bytes_per_pixel) {
    if (bytes_per_pixel == 1) {
        switch (bayer_pattern) {
        case bmpRG:
            return bot_core::image_t::PIXEL_FORMAT_BAYER_RGGB;   //BAYER_RGGB8;
        case bmpGB:
            return bot_core::image_t::PIXEL_FORMAT_BAYER_GBRG;   //BAYER_GBRG8;
        case bmpGR:
            return bot_core::image_t::PIXEL_FORMAT_BAYER_GRBG;   //BAYER_GRBG8;
        case bmpBG:
            return bot_core::image_t::PIXEL_FORMAT_BAYER_BGGR;   //BAYER_BGGR8;
        default:
            return bot_core::image_t::PIXEL_FORMAT_GRAY;
        }
    }
    else if (bytes_per_pixel == 2) {
        switch (bayer_pattern) {
        case bmpRG:
            return bot_core::image_t::PIXEL_FORMAT_BE_BAYER16_RGGB; //BAYER_RGGB16;
        case bmpGB:
            return bot_core::image_t::PIXEL_FORMAT_BE_BAYER16_GBRG; //BAYER_GBRG16;
        case bmpGR:
            return bot_core::image_t::PIXEL_FORMAT_BE_BAYER16_GRBG; //BAYER_GRBG16;
        case bmpBG:
            return bot_core::image_t::PIXEL_FORMAT_BE_BAYER16_BGGR; //BAYER_BGGR16;
        default:
            return bot_core::image_t::PIXEL_FORMAT_BE_GRAY16;   //MONO16;
        }
    }
    return bot_core::image_t::PIXEL_FORMAT_GRAY;    //MONO8;
}

// http://www.matrix-vision.com/manuals/mvBlueFOX/Appendix_page_0.html#CMOS752_section_1_1
double PixelClockToFrameRate(int pclk_khz, double width, double height,
                             double expose_us) 
{
    static const double kTriggerPulseWidthUs = 200;
    double frame_time_us = (width + 61) * (height + 45) / pclk_khz * 1e3;
    return 1e6 / (frame_time_us + expose_us + kTriggerPulseWidthUs);
}

}