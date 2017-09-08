#include "Parser.h"

const string bluefox2cfg_path("../share/config/bluefox2-mono.json");

Bluefox2Parser::Bluefox2Parser()
{
    InitParser();
    SetAllConfig();
}

Bluefox2Parser::~Bluefox2Parser()
{

}

void Bluefox2Parser::InitParser()
{
    _param = new IpmsParam(bluefox2cfg_path);

    _serial = _param->param_get_string("camera.serial");

    ibpf_hash = boost::assign::map_list_of("ibpfAuto", ibpfAuto)("ibpfRaw", ibpfRaw)("ibpfMono8", ibpfMono8)("ibpfMono16", ibpfMono16)
                                          ("ibpfRGBx888Packed", ibpfRGBx888Packed)("ibpfYUV422Packed", ibpfYUV422Packed)("ibpfRGBx888Planar",ibpfRGBx888Planar)
                                          ("ibpfMono10", ibpfMono10)("ibpfMono12", ibpfMono12)("ibpfMono14", ibpfMono14)
                                          ("ibpfRGB888Packed", ibpfRGB888Packed)("ibpfYUV444Planar", ibpfYUV444Planar)("ibpfMono32", ibpfMono32)
                                          ("ibpfYUV422Planar", ibpfYUV422Planar)("ibpfRGB101010Packed", ibpfRGB101010Packed)("ibpfRGB121212Packed", ibpfRGB121212Packed)
                                          ("ibpfRGB141414Packed", ibpfRGB141414Packed)("ibpfRGB161616Packed", ibpfRGB161616Packed)
                                          ("ibpfYUV422_UYVYPacked", ibpfYUV422_UYVYPacked)("ibpfMono12Packed_V2", ibpfMono12Packed_V2)
                                          ("ibpfYUV422_10Packed", ibpfYUV422_10Packed)("ibpfYUV422_UYVY_10Packed", ibpfYUV422_UYVY_10Packed)
                                          ("ibpfBGR888Packed", ibpfBGR888Packed)("ibpfBGR101010Packed_V2", ibpfBGR101010Packed_V2)
                                          ("ibpfYUV444_UYVPacked", ibpfYUV444_UYVPacked)("ibpfYUV444_UYV_10Packed", ibpfYUV444_UYV_10Packed)
                                          ("ibpfYUV444Packed", ibpfYUV444Packed)("ibpfYUV444_10Packed", ibpfYUV444_10Packed)
                                          ("ibpfMono12Packed_V1", ibpfMono12Packed_V1)("ibpfYUV411_UYYVYY_Packed", ibpfYUV411_UYYVYY_Packed)
                                          ("ibpfRGB888Planar", ibpfRGB888Planar).convert_to_container< unordered_map<string, ibpf_enum> >();

    cbm_hash = boost::assign::map_list_of("cbmOff", cbmOff)("cbmBinningH", cbmBinningH)("cbmBinningV", cbmBinningV)("cbmBinningHV", cbmBinningHV)
                                         ("cbmBinning3H", cbmBinning3H)("cbmBinning3V", cbmBinning3V)("cbmBinning3H3V", cbmBinning3H3V)("cbmBinningHAvg", cbmBinningHAvg)
                                         ("cbmBinningVAvg", cbmBinningVAvg)("cbmBinning3HAvg", cbmBinning3HAvg)("cbmBinning3VAvg", cbmBinning3VAvg)
                                         ("cbmBinning3H3VAvg", cbmBinning3H3VAvg)("cbmDroppingH", cbmDroppingH)("cbmDroppingV", cbmDroppingV)
                                         ("cbmDroppingHV", cbmDroppingHV)("cbmDropping3H", cbmDropping3H)("cbmDropping3V", cbmDropping3V)
                                         ("cbmDropping3H3V", cbmDropping3H3V).convert_to_container< unordered_map<string, cbm_enum> >();

    acs_hash = boost::assign::map_list_of("acs_unavailable", acs_unavailable)("acs_slow", acs_slow)("acs_medium", acs_medium)("acs_fast", acs_fast).convert_to_container< unordered_map<string, acs_enum> >();

    wbp_hash = boost::assign::map_list_of("wbp_unavailable", wbp_unavailable)("wbp_tungsten", wbp_tungsten)("wbp_haloge", wbp_haloge)("wbp_fluorescent", wbp_fluorescent)
                                         ("wbp_daylight", wbp_daylight)("wbp_photolight", wbp_photolight)("wbp_bluesky", wbp_bluesky)("wbp_user1", wbp_user1)
                                         ("wbp_calibrate", wbp_calibrate).convert_to_container< unordered_map<string, wbp_enum> >();

    dcfm_hash = boost::assign::map_list_of("dcfm_off", dcfm_off)("dcfm_on", dcfm_on)("dcfm_calibrate", dcfm_calibrate)("correction_image", correction_image).convert_to_container< unordered_map<string, dcfm_enum> >();

    cpc_hash = boost::assign::map_list_of("cpc_12000", cpc_12000)("cpc_20000", cpc_20000)("cpc_24000", cpc_24000)("cpc_27000", cpc_27000)("cpc_32000", cpc_32000)("cpc_40000", cpc_40000)("cpc_50000", cpc_50000).convert_to_container< unordered_map<string, cpc_enum> >();

    ctm_hash = boost::assign::map_list_of("ctm_continuous", ctm_continuous)("ctm_on_dement", ctm_on_dement)("ctm_on_low_level", ctm_on_low_level)("ctm_on_high_level", ctm_on_high_level)("ctm_on_falling_edge", ctm_on_falling_edge)("ctm_on_rising_edge", ctm_on_rising_edge)("hard_sync", hard_sync).convert_to_container< unordered_map<string, ctm_enum> >();

    cts_hash = boost::assign::map_list_of("cts_unavailable", cts_unavailable)("cts_dig_in_0", cts_dig_in_0)("cts_dig_in_1", cts_dig_in_1)("cts_RTCtrl", cts_RTCtrl).convert_to_container< unordered_map<string, cts_enum> >();

    cem_hash = boost::assign::map_list_of("cemStandard", cemStandard)("cemOverlapped", cemOverlapped)("cemNoShutter", cemNoShutter).convert_to_container< unordered_map<string, cem_enum> >();

    cticmm_hash = boost::assign::map_list_of("cticmmUser", cticmmUser)("cticmmDeviceSpecific", cticmmDeviceSpecific)("cticmmBlueFOX_x00wC_WPPLS", cticmmBlueFOX_x00wC_WPPLS).convert_to_container< unordered_map<string, cticmm_enum> >();

    ctocmm_hash = boost::assign::map_list_of("ctocmmUser", ctocmmUser)("ctocmmXYZToAdobeRGB_D50", ctocmmXYZToAdobeRGB_D50)("ctocmmXYZTosRGB_D50", ctocmmXYZTosRGB_D50)("ctocmmXYZToWideGamutRGB_D50", ctocmmXYZToWideGamutRGB_D50)("ctocmmXYZToAdobeRGB_D65", ctocmmXYZToAdobeRGB_D65)("ctocmmXYZTosRGB_D65", ctocmmXYZTosRGB_D65).convert_to_container< unordered_map<string, ctocmm_enum> >();

    lutmode_hash = boost::assign::map_list_of("LUTmInterpolated", LUTmInterpolated)("LUTmGamma", LUTmGamma)("LUTmDirect", LUTmDirect).convert_to_container < unordered_map<string, lutmode_enum> >();

    lutimpl_hash = boost::assign::map_list_of("LUTiHardware", LUTiHardware)("LUTiSoftware", LUTiSoftware).convert_to_container< unordered_map<string, lutimpl_enum> >();

    lutmapping_hash = boost::assign::map_list_of("LUTm8To8", LUTm8To8)("LUTm10To8", LUTm10To8)("LUTm10To10", LUTm10To10)("LUTm12To10", LUTm12To10)("LUTm12To12", LUTm12To12)("LUTm14To14", LUTm14To14)("LUTm16To16", LUTm16To16).convert_to_container< unordered_map<string, lutmapping_enum> >();

    cfm_hash = boost::assign::map_list_of("cfmOff", cfmOff)("cfmDigout0", cfmDigout0)("cfmDigout1", cfmDigout1).convert_to_container< unordered_map<string, cfm_enum> >();

    cft_hash = boost::assign::map_list_of("cftStandard", cftStandard)("cftRollingShutterFlash", cftRollingShutterFlash)("cftVSync", cftVSync).convert_to_container< unordered_map<string, cft_enum> >();

}

void Bluefox2Parser::SetAllConfig()
{
    SetSerial();
    SetWidth();
    SetHeight();
    SetIdpf();
    SetCbm();
    SetAgc();
    SetGainDB();
    SetAec();
    SetExposure();
    SetAcs();
    SetDesGrayVal();
    SetWbp();
    SetRGain();
    SetGGain();
    SetBGain();
    SetHDR();
    SetDcfm();
    SetCpc();
    SetCtm();
    SetCts();
    SetRequest();
    SetCem();
    SetCticme();
    SetCticmm();
    SetCte();
    SetCtr();
    SetCtocme();
    SetCtocmm();
    SetLUTe();
    SetLUTmode();
    SetLUTimpl();
    SetLUTmapping();    
    SetCfm();
    SetCft();
}

void Bluefox2Parser::SetIdpf()
{
    string idpfkey = _param->param_get_string("camera.config.idpf");
    _config.idpf = ibpf_hash[idpfkey];
}

void Bluefox2Parser::SetCbm()
{
    string cbmkey = _param->param_get_string("camera.config.cbm");
    _config.cbm = cbm_hash[cbmkey];
}

void Bluefox2Parser::SetAcs()
{
    string acskey = _param->param_get_string("camera.config.acs");
    _config.acs = acs_hash[acskey];
}

void Bluefox2Parser::SetWbp()
{
    string wbpkey = _param->param_get_string("camera.config.wbp");
    _config.wbp = wbp_hash[wbpkey];
}

void Bluefox2Parser::SetDcfm()
{
    string dcfmkey = _param->param_get_string("camera.config.dcfm");
    _config.dcfm = dcfm_hash[dcfmkey];
}

void Bluefox2Parser::SetCpc()
{
    string cpckey = _param->param_get_string("camera.config.cpc");
    _config.cpc = cpc_hash[cpckey];
}

void Bluefox2Parser::SetCtm()
{
    string ctmkey = _param->param_get_string("camera.config.ctm");
    _config.ctm = ctm_hash[ctmkey];
}

void Bluefox2Parser::SetCts()
{
    string ctskey = _param->param_get_string("camera.config.cts");
    _config.cts = cts_hash[ctskey];
}

void Bluefox2Parser::SetCem()
{
    string cemkey = _param->param_get_string("camera.config.cem");
    _config.cem = cem_hash[cemkey];
}

void Bluefox2Parser::SetCticmm()
{
    string cticmmkey = _param->param_get_string("camera.config.cticmm");
    _config.cticmm = cticmm_hash[cticmmkey];
}

void Bluefox2Parser::SetCtr()
{

    _vec_ctrrow0 = _param->param_get_double_array("camera.config.ctr.row0");
    _vec_ctrrow1 = _param->param_get_double_array("camera.config.ctr.row1");
    _vec_ctrrow2 = _param->param_get_double_array("camera.config.ctr.row2");

    _config.ctrrow0 = _vec_ctrrow0;
    _config.ctrrow1 = _vec_ctrrow1;
    _config.ctrrow2 = _vec_ctrrow2;

}

void Bluefox2Parser::SetCtocmm()
{
    string ctocmmkey = _param->param_get_string("camera.config.ctocmm");
    _config.ctocmm = ctocmm_hash[ctocmmkey];
}

void Bluefox2Parser::SetLUTmode()
{
    string lutmodekey = _param->param_get_string("camera.config.lutmode");
    _config.lutmode = lutmode_hash[lutmodekey];
}

void Bluefox2Parser::SetLUTimpl()
{
    string lutimplkey = _param->param_get_string("camera.config.lutimpl");
    _config.lutimpl = lutimpl_hash[lutimplkey];
}

void Bluefox2Parser::SetLUTmapping()
{
    string lutmappingkey = _param->param_get_string("camera.config.lutmapping");
    _config.lutmapping = lutmapping_hash[lutmappingkey];
}

void Bluefox2Parser::SetCfm()
{
    string cfmkey = _param->param_get_string("camera.config.cfm");
    _config.cfm = cfm_hash[cfmkey];
}

void Bluefox2Parser::SetCft()
{
    string cftkey = _param->param_get_string("camera.config.cft");
    _config.cft = cft_hash[cftkey];
}