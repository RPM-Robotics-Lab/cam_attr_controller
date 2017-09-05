#pragma once
#include <string>
#include <unordered_map>
#include <boost/assign/list_of.hpp>

#include "param-util/param_util.h"
#include "Bluefox2Driver/bluefox2_enumerator.h"
#include "Bluefox2Driver/Bluefox2Config.h"

using namespace std;
using namespace ipms_param;
using namespace bluefox2;

class Bluefox2Parser {

public:
    Bluefox2Parser();
    ~Bluefox2Parser();

    string GetSerial() { return _serial; }
    Bluefox2Config GetConfig(){ return _config; }

private:
    IpmsParam *_param;

    unordered_map<string, ibpf_enum> ibpf_hash;
    unordered_map<string, cbm_enum> cbm_hash;
    unordered_map<string, acs_enum> acs_hash;
    unordered_map<string, wbp_enum> wbp_hash;
    unordered_map<string, dcfm_enum> dcfm_hash;
    unordered_map<string, cpc_enum> cpc_hash;
    unordered_map<string, ctm_enum> ctm_hash;
    unordered_map<string, cts_enum> cts_hash;
    unordered_map<string, cem_enum> cem_hash;
    unordered_map<string, cticmm_enum> cticmm_hash;
    unordered_map<string, ctocmm_enum> ctocmm_hash;
    unordered_map<string, lutmode_enum> lutmode_hash;
    unordered_map<string, lutimpl_enum> lutimpl_hash;
    unordered_map<string, lutmapping_enum> lutmapping_hash;
    unordered_map<string, cfm_enum> cfm_hash;
    unordered_map<string, cft_enum> cft_hash;

    vector<double> _vec_ctrrow0;
    vector<double> _vec_ctrrow1;
    vector<double> _vec_ctrrow2;

    
    string _serial;
    Bluefox2Config _config;

    void InitParser();

    void SetAllConfig();

    void SetSerial() {
        _serial = _param->param_get_string("camera.serial");
    }

    void SetWidth() {
        _config.width = _param->param_get_int("camera.config.width");
    }

    void SetHeight() {
        _config.height = _param->param_get_int("camera.config.height");
    }

    void SetIdpf();

    void SetCbm();
    
    void SetAgc() {
        _config.agc = _param->param_get_boolean("camera.config.agc");
    }

    void SetGainDB() {
        _config.gain_db = _param->param_get_double("camera.config.gain_db");
    };

    void SetAec() {
        _config.aec = _param->param_get_boolean("camera.config.aec");
    }

    void SetExposure() {
        _config.expose_us = _param->param_get_int("camera.config.expose_us");
    }
    
    void SetAcs();

    void SetDesGrayVal() {
        _config.des_gray_val = _param->param_get_int("camera.config.des_gray_val");
    }

    void SetWbp();

    void SetRGain() {
        _config.r_gain = _param->param_get_double("camera.config.r_gain");
    }

    void SetGGain() {
        _config.g_gain = _param->param_get_double("camera.config.g_gain");
    }

    void SetBGain() {
        _config.b_gain = _param->param_get_double("camera.config.b_gain");
    }

    void SetHDR() {
        _config.hdr = _param->param_get_boolean("camera.config.hdr");
    }

    void SetDcfm();
    
    void SetCpc();
    
    void SetCtm();
    
    void SetCts();
    
    void SetRequest() {
        _config.request = _param->param_get_int("camera.config.request");
    }
    
    void SetCem();

    void SetCticme() {
        _config.cticme = _param->param_get_boolean("camera.config.cticme");
    }

    void SetCticmm();

    void SetCte() {
        _config.cte = _param->param_get_boolean("camera.config.cte");
    }

    void SetCtr();

    void SetCtocme() {
        _config.ctocme = _param->param_get_boolean("camera.config.ctocme");
    }

    void SetCtocmm();

    void SetLUTe() {
        _config.lute = _param->param_get_boolean("camera.config.lute");
    }

    void SetLUTmode();
    void SetLUTimpl();
    void SetLUTmapping();

    void SetCfm();
    void SetCft();

};