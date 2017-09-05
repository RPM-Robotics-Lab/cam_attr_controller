#pragma once
#include "param-util/param_util.h"
#include "Bluefox2Driver/Bluefox2Config.h"

using namespace ipms_param;
using namespace bluefox2;

class Bluefox2Parser {

public:
    Bluefox2Parser();
    Bluefox2Parser(string side);
    ~Bluefox2Parser();

    string GetSerial() { return _serial; }
    Bluefox2Config GetConfig(){ return _config; }

private:
    IpmsParam *_param;
    string _serial;
    Bluefox2Config _config;

    string _side;

    vector<string> _vec_ibpftype;
    vector<int> _vec_ibpfid;

    vector<string> _vec_cbmtype;
    vector<int> _vec_cbmid;    

    vector<string> _vec_acstype;
    vector<int> _vec_acsid;    

    vector<string> _vec_wbptype;
    vector<int> _vec_wbpid;    

    vector<string> _vec_dcfmtype;
    vector<int> _vec_dcfmid;    


    vector<string> _vec_cpctype;
    vector<int> _vec_cpcid;    

    vector<string> _vec_ctmtype;
    vector<int> _vec_ctmid;

    vector<string> _vec_ctstype;
    vector<int> _vec_ctsid;

    vector<string> _vec_cemtype;
    vector<int> _vec_cemid;

    vector<string> _vec_cticmmtype;
    vector<int> _vec_cticmmid;

    vector<double> _vec_ctrrow0;
    vector<double> _vec_ctrrow1;
    vector<double> _vec_ctrrow2;

    vector<string> _vec_ctocmmtype;
    vector<int> _vec_ctocmmid;

    vector<string> _vec_lutmodetype;
    vector<int> _vec_lutmodeid;

    vector<string> _vec_lutimpltype;
    vector<int> _vec_lutimplid;

    vector<string> _vec_lutmappingtype;
    vector<int> _vec_lutmappingid;

    vector<string> _vec_cfmtype;
    vector<int> _vec_cfmid;

    vector<string> _vec_cfttype;
    vector<int> _vec_cftid;

    void InitParser();

    void SetAllConfig();

    void SetSerial() {
        _serial = _param->param_get_string(_side+"-camera.serial");
    }

    void SetWidth() {
        _config.width = _param->param_get_int(_side+"-camera.config.width");
    }

    void SetHeight() {
        _config.height = _param->param_get_int(_side+"-camera.config.height");
    }

    void SetIdpf();

    void SetCbm();
    
    void SetAgc() {
        _config.agc = _param->param_get_boolean(_side+"-camera.config.agc");
    }

    void SetGainDB() {
        _config.gain_db = _param->param_get_double(_side+"-camera.config.gain_db");
    };

    void SetAec() {
        _config.aec = _param->param_get_boolean(_side+"-camera.config.aec");
    }

    void SetExposure() {
        _config.expose_us = _param->param_get_int(_side+"-camera.config.expose_us");
    }
    
    void SetAcs();

    void SetDesGrayVal() {
        _config.des_gray_val = _param->param_get_int(_side+"-camera.config.des_gray_val");
    }

    void SetWbp();

    void SetRGain() {
        _config.r_gain = _param->param_get_double(_side+"-camera.config.r_gain");
    }

    void SetGGain() {
        _config.g_gain = _param->param_get_double(_side+"-camera.config.g_gain");
    }

    void SetBGain() {
        _config.b_gain = _param->param_get_double(_side+"-camera.config.b_gain");
    }

    void SetHDR() {
        _config.hdr = _param->param_get_boolean(_side+"-camera.config.hdr");
    }

    void SetDcfm();
    
    void SetCpc();
    
    void SetCtm();
    
    void SetCts();
    
    void SetRequest() {
        _config.request = _param->param_get_int(_side+"-camera.config.request");
    }
    
    void SetCem();

    void SetCticme() {
        _config.cticme = _param->param_get_boolean(_side+"-camera.config.cticme");
    }

    void SetCticmm();

    void SetCte() {
        _config.cte = _param->param_get_boolean(_side+"-camera.config.cte");
    }

    void SetCtr();

    void SetCtocme() {
        _config.ctocme = _param->param_get_boolean(_side+"-camera.config.ctocme");
    }

    void SetCtocmm();

    void SetLUTe() {
        _config.lute = _param->param_get_boolean(_side+"-camera.config.lute");
    }

    void SetLUTmode();
    void SetLUTimpl();
    void SetLUTmapping();

    void SetCfm();
    void SetCft();

    int find_enum_index(vector<string>& vec_enum_type, string key);
    int get_enum_val(vector<int>& vec_id, int index);
};