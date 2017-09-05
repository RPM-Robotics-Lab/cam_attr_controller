#include "Bluefox2Parser.h"

const string bluefox2cfg_path("../share/config/bluefox-stereo.json");

Bluefox2Parser::Bluefox2Parser()
{
    _side = string("left");

    InitParser();
    SetAllConfig();
}

Bluefox2Parser::Bluefox2Parser(string side)
{
    _side = side;
    cout << "[Bluefox2Parser]\t" << side+"-camera constructor" << endl;

    InitParser();
    SetAllConfig();
}

Bluefox2Parser::~Bluefox2Parser()
{

}

void Bluefox2Parser::InitParser()
{
    _param = new IpmsParam(bluefox2cfg_path);

    _vec_ibpftype = _param->param_get_str_array("enumerator.ibpf.name");
    _vec_ibpfid = _param->param_get_int_array("enumerator.ibpf.id");

    _vec_cbmtype = _param->param_get_str_array("enumerator.cbm.name");
    _vec_cbmid = _param->param_get_int_array("enumerator.cbm.id");

    _vec_acstype = _param->param_get_str_array("enumerator.acs.name");
    _vec_acsid = _param->param_get_int_array("enumerator.acs.id");

    _vec_wbptype = _param->param_get_str_array("enumerator.wbp.name");
    _vec_wbpid = _param->param_get_int_array("enumerator.wbp.id");

    _vec_dcfmtype = _param->param_get_str_array("enumerator.dcfm.name");
    _vec_dcfmid = _param->param_get_int_array("enumerator.dcfm.id");

    _vec_cpctype = _param->param_get_str_array("enumerator.cpc.name");
    _vec_cpcid = _param->param_get_int_array("enumerator.cpc.id");

    _vec_ctmtype = _param->param_get_str_array("enumerator.ctm.name");
    _vec_ctmid = _param->param_get_int_array("enumerator.ctm.id");

    _vec_ctstype = _param->param_get_str_array("enumerator.cts.name");
    _vec_ctsid = _param->param_get_int_array("enumerator.cts.id");

    _vec_cemtype = _param->param_get_str_array("enumerator.cem.name");
    _vec_cemid = _param->param_get_int_array("enumerator.cem.id");

    _vec_cticmmtype = _param->param_get_str_array("enumerator.cticmm.name");
    _vec_cticmmid = _param->param_get_int_array("enumerator.cticmm.id");

    _vec_ctocmmtype = _param->param_get_str_array("enumerator.ctocmm.name");
    _vec_ctocmmid = _param->param_get_int_array("enumerator.ctocmm.id");

    _vec_lutmodetype = _param->param_get_str_array("enumerator.lutmode.name");
    _vec_lutmodeid = _param->param_get_int_array("enumerator.lutmode.id");

    _vec_lutimpltype = _param->param_get_str_array("enumerator.lutimpl.name");
    _vec_lutimplid = _param->param_get_int_array("enumerator.lutimpl.id");

    _vec_lutmappingtype = _param->param_get_str_array("enumerator.lutmapping.name");
    _vec_lutmappingid = _param->param_get_int_array("enumerator.lutmapping.id");    

    _vec_cfmtype = _param->param_get_str_array("enumerator.cfm.name");
    _vec_cfmid = _param->param_get_int_array("enumerator.cfm.id");

    _vec_cfttype = _param->param_get_str_array("enumerator.cft.name");
    _vec_cftid = _param->param_get_int_array("enumerator.cft.id");
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
    string idpfkey = _param->param_get_string(_side+"-camera.config.idpf");
    int index = find_enum_index(_vec_ibpftype, idpfkey);
    _config.idpf = get_enum_val(_vec_ibpfid, index);
}

void Bluefox2Parser::SetCbm()
{
    string cbmkey = _param->param_get_string(_side+"-camera.config.cbm");
    int index = find_enum_index(_vec_cbmtype, cbmkey);
    _config.cbm = get_enum_val(_vec_cbmid, index);    
}

void Bluefox2Parser::SetAcs()
{
    string acskey = _param->param_get_string(_side+"-camera.config.acs");
    int index = find_enum_index(_vec_acstype, acskey);
    _config.acs = get_enum_val(_vec_acsid, index);    
}

void Bluefox2Parser::SetWbp()
{
    string wbpkey = _param->param_get_string(_side+"-camera.config.wbp");
    int index = find_enum_index(_vec_wbptype, wbpkey);
    _config.wbp = get_enum_val(_vec_wbpid, index);    
}

void Bluefox2Parser::SetDcfm()
{
    string dcfmkey = _param->param_get_string(_side+"-camera.config.dcfm");
    int index = find_enum_index(_vec_dcfmtype, dcfmkey);
    _config.dcfm = get_enum_val(_vec_dcfmid, index);    
}

void Bluefox2Parser::SetCpc()
{
    string cpckey = _param->param_get_string(_side+"-camera.config.cpc");
    int index = find_enum_index(_vec_cpctype, cpckey);
    _config.cpc = get_enum_val(_vec_cpcid, index);    
}

void Bluefox2Parser::SetCtm()
{
    string ctmkey = _param->param_get_string(_side+"-camera.config.ctm");
    int index = find_enum_index(_vec_ctmtype, ctmkey);
    _config.ctm = get_enum_val(_vec_ctmid, index);    
}

void Bluefox2Parser::SetCts()
{
    string ctskey = _param->param_get_string(_side+"-camera.config.cts");
    int index = find_enum_index(_vec_ctstype, ctskey);
    _config.cts = get_enum_val(_vec_ctsid, index);    
}

void Bluefox2Parser::SetCem()
{
    string cemkey = _param->param_get_string(_side+"-camera.config.cem");
    int index = find_enum_index(_vec_cemtype, cemkey);
    _config.cem = get_enum_val(_vec_cemid, index);    
}

void Bluefox2Parser::SetCticmm()
{
    string cticmmkey = _param->param_get_string(_side+"-camera.config.cticmm");
    int index = find_enum_index(_vec_cticmmtype, cticmmkey);
    _config.cticmm = get_enum_val(_vec_cticmmid, index);    
}

void Bluefox2Parser::SetCtr()
{

    _vec_ctrrow0 = _param->param_get_double_array("enumerator.ctr.row0");
    _vec_ctrrow1 = _param->param_get_double_array("enumerator.ctr.row1");
    _vec_ctrrow2 = _param->param_get_double_array("enumerator.ctr.row2");

    _config.ctrrow0 = _vec_ctrrow0;
    _config.ctrrow1 = _vec_ctrrow1;
    _config.ctrrow2 = _vec_ctrrow2;

}

void Bluefox2Parser::SetCtocmm()
{
    string ctocmmkey = _param->param_get_string(_side+"-camera.config.ctocmm");
    int index = find_enum_index(_vec_ctocmmtype, ctocmmkey);
    _config.ctocmm = get_enum_val(_vec_ctocmmid, index);    
}

void Bluefox2Parser::SetLUTmode()
{
    string lutmodekey = _param->param_get_string(_side+"-camera.config.lutmode");
    int index = find_enum_index(_vec_lutmodetype, lutmodekey);
    _config.lutmode = get_enum_val(_vec_lutmodeid, index);    
}

void Bluefox2Parser::SetLUTimpl()
{
    string lutimplkey = _param->param_get_string(_side+"-camera.config.lutimpl");
    int index = find_enum_index(_vec_lutimpltype, lutimplkey);
    _config.lutimpl = get_enum_val(_vec_lutimplid, index);    
}

void Bluefox2Parser::SetLUTmapping()
{
    string lutmappingkey = _param->param_get_string(_side+"-camera.config.lutmapping");
    int index = find_enum_index(_vec_lutmappingtype, lutmappingkey);
    _config.lutmapping = get_enum_val(_vec_lutmappingid, index);    
}

void Bluefox2Parser::SetCfm()
{
    string cfmkey = _param->param_get_string(_side+"-camera.config.cfm");
    int index = find_enum_index(_vec_cfmtype, cfmkey);
    _config.cfm = get_enum_val(_vec_cfmid, index);    
}

void Bluefox2Parser::SetCft()
{
    string cftkey = _param->param_get_string(_side+"-camera.config.cft");
    int index = find_enum_index(_vec_cfttype, cftkey);
    _config.cft = get_enum_val(_vec_cftid, index);    
}

int Bluefox2Parser::find_enum_index(vector<string>& vec_enum_type, string key)
{
    vector<string>::iterator it;
    it = find(vec_enum_type.begin(), vec_enum_type.end(), key);
    int index = it - vec_enum_type.begin();
    // cout << "Finded Key: " << *it << endl;
    // cout << "Index: " << index << endl;

    return index;
}

int Bluefox2Parser::get_enum_val(vector<int>& vec_id, int index)
{
    return vec_id[index];
}