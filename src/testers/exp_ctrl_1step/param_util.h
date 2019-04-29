#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <string>

#include <boost/lexical_cast.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#define PARAM_DEFAULT_CFG "../share/config/master_cfg.json"

using namespace std;
using boost::property_tree::ptree;

namespace ipms_param {

class IpmsParam {
public:
    explicit IpmsParam(const string &fileName);
    
    ~IpmsParam();

    template<typename C1, typename C2>
    void castContainer(const C1& source, C2& destination)
    {
        typedef typename C1::value_type source_type;
        typedef typename C2::value_type destination_type;
        destination.resize(source.size());
        transform(source.begin(), source.end(), destination.begin(), boost::lexical_cast<destination_type, source_type>);
    }

    bool param_get_boolean (const string &key) { return param_.get<bool> (key); }
    int param_get_int (const string &key) { return param_.get<int> (key); }
    float param_get_float (const string &key) { return param_.get<float> (key); }
    double param_get_double (const string &key) { return param_.get<double> (key); }
    string param_get_string (const string &key) { return param_.get<string> (key); }

    vector<int> param_get_int_array (const string &key);
    vector<float> param_get_float_array (const string &key);
    vector<double> param_get_double_array (const string &key);
    vector<string> param_get_str_array (const string &key);

private:
    string fileName_;
    ptree param_;
};

}