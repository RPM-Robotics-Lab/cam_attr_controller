#pragma once
using namespace std;

namespace bluefox2 {

// input pixel format  
enum ibpf_enum
{
    ibpfAuto = -1,
    ibpfRaw,
    ibpfMono8,
    ibpfMono16,
    ibpfRGBx888Packed,
    ibpfYUV422Packed,
    ibpfRGBx888Planar,
    ibpfMono10,
    ibpfMono12,
    ibpfMono14,
    ibpfRGB888Packed,
    ibpfYUV444Planar,
    ibpfMono32,
    ibpfYUV422Planar,
    ibpfRGB101010Packed,
    ibpfRGB121212Packed,
    ibpfRGB141414Packed,
    ibpfRGB161616Packed,
    ibpfYUV422_UYVYPacked,
    ibpfMono12Packed_V2,
    ibpfYUV422_10Packed = 20,
    ibpfYUV422_UYVY_10Packed = 21,
    ibpfBGR888Packed = 22,
    ibpfBGR101010Packed_V2 = 23,
    ibpfYUV444_UYVPacked = 24,
    ibpfYUV444_UYV_10Packed = 25,
    ibpfYUV444Packed = 26,
    ibpfYUV444_10Packed = 27,
    ibpfMono12Packed_V1 = 28,
    ibpfYUV411_UYYVYY_Packed = 29,
    ibpfRGB888Planar = 30
};

// Binning
enum cbm_enum
{   //0,1,2,3,16,32,48,101,102,257,258,272,288,304,4097,4098,4099,4112,4128,4144
    cbmOff = 0,
    cbmBinningH = 1,
    cbmBinningV = 2,
    cbmBinningHV = 3,
    cbmBinning3H = 16,
    cbmBinning3V = 32,
    cbmBinning3H3V = 48,
    cbmBinningHAvg = 101,
    cbmBinningVAvg = 102,
    cbmBinning3HAvg = 257,
    cbmBinning3VAvg = 258,
    cbmBinning3H3VAvg = 272,
    cbmDroppingH = 288,
    cbmDroppingV = 304,
    cbmDroppingHV = 4097,
    cbmDropping3H = 4098,
    cbmDropping3V = 4099,
    cbmDropping3H3V = 4112
};

// enum for 
// enum for Auto Cotroller
enum acs_enum
{
    acs_unavailable = -1,
    acs_slow,
    acs_medium,
    acs_fast
};

// enum for White Balance
enum wbp_enum
{
    wbp_unavailable = -1,
    wbp_tungsten = 0,
    wbp_haloge = 1,
    wbp_fluorescent = 2,
    wbp_daylight = 3,
    wbp_photolight = 4,
    wbp_bluesky = 5,
    wbp_user1 = 6,
    wbp_calibrate = 10
};

// enum for Dark Current Filter Mode
enum dcfm_enum
{
    dcfm_off = 0,
    dcfm_on = 1,
    dcfm_calibrate = 2,
    correction_image = 3
};

// enum for Camera Pixel Clock
enum cpc_enum
{
    cpc_12000 = 12000,
    cpc_20000 = 20000,
    cpc_24000 = 24000,
    cpc_27000 = 27000,
    cpc_32000 = 32000,
    cpc_40000 = 40000,
    cpc_50000 = 50000
};

// enum for Camera Trigger Mode
enum ctm_enum
{
    ctm_continuous = 0,
    ctm_on_dement = 1,
    ctm_on_low_level = 2,
    ctm_on_high_level = 3,
    ctm_on_falling_edge = 4,
    ctm_on_rising_edge = 5,
    hard_sync = -1
};

// enum for Camera Trigger Source
enum cts_enum
{
    cts_unavailable = -1,
    cts_dig_in_0 = 0,
    cts_dig_in_1 = 1,
    cts_RTCtrl = 2
};

// enum for Exposure Mode
enum cem_enum
{
    cemStandard = 0,
    cemOverlapped = 1,
    cemNoShutter = 2
};

// enum for Color Twist Input Correction Matrix Mode
enum cticmm_enum
{
    cticmmUser = 69632,
    cticmmDeviceSpecific = 73728,
    cticmmBlueFOX_x00wC_WPPLS = 268697601
};

// enum for Color Twist Output Correction Matrix Mode
enum ctocmm_enum
{
    ctocmmUser = 0,
    ctocmmXYZToAdobeRGB_D50 = 1,
    ctocmmXYZTosRGB_D50 = 2,
    ctocmmXYZToWideGamutRGB_D50 = 3,
    ctocmmXYZToAdobeRGB_D65 = 4,
    ctocmmXYZTosRGB_D65 = 5
};

//enum for LUT mode
enum lutmode_enum
{
    LUTmInterpolated = 0,
    LUTmGamma,
    LUTmDirect
};

//enum for LUT impl
enum lutimpl_enum
{
    LUTiHardware = 0,
    LUTiSoftware
};

//enum for lutmapping
enum lutmapping_enum
{
    LUTm8To8 = 524296,
    LUTm10To8 = 655368, 
    LUTm10To10 = 655370,
    LUTm12To10 = 786442,
    LUTm12To12 = 786444,
    LUTm14To14 = 917518,
    LUTm16To16 = 1048592
};

// enum for Camera Flash Mode
enum cfm_enum
{
    cfmOff = 0,
    cfmDigout0,
    cfmDigout1
};

// enum for Camera Flash Type
enum cft_enum
{
    cftStandard = 0,
    cftRollingShutterFlash,
    cftVSync
};

} // namespace bluefox2