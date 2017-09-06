#include <iostream>
#include <boost/lexical_cast.hpp>

#include "param_util.h"

using namespace std;

namespace ipms_param {

IpmsParam::IpmsParam(const string &fileName) : fileName_(fileName) {
    cerr << "[IpmsParam]\tInput config file name : " << fileName_ << endl;
    boost::property_tree::read_json(fileName_, param_);
}

IpmsParam::~IpmsParam() 
{
}

vector<int> IpmsParam::param_get_int_array (const string &key)
{
    vector<string> vString = param_get_str_array(key);

    vector<int> vInt;
    castContainer(vString, vInt);

    return vInt;
}

vector<float> IpmsParam::param_get_float_array (const string &key)
{
    vector<string> vString = param_get_str_array(key);
    
    vector<float> vFloat;
    castContainer(vString, vFloat);

    return vFloat;
}

vector<double> IpmsParam::param_get_double_array (const string &key)
{
    vector<string> vString = param_get_str_array(key);
    
    vector<double> vDouble;
    castContainer(vString, vDouble);

    return vDouble;
}

vector<string> IpmsParam::param_get_str_array (const string &key)
{
    vector<string> vString;
    for (auto &strVal : param_.get_child(key)) {
        vString.push_back(strVal.second.data());
    }

    return vString;
}

}
