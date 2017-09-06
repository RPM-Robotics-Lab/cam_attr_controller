#pragma once
#include <vector>

#include "bluefox2_enumerator.h"

using namespace std;

namespace bluefox2 {
   
class Bluefox2Config{
public:
    Bluefox2Config();
    ~Bluefox2Config();

    // Area of Interset
    int width;
    int height;

    // Pixel Format
    int idpf;

    // Binning
    int cbm;

    // Gain
    bool agc;
    double gain_db;

    // Expose
    bool aec;
    int expose_us;

    // Auto Controller
    int acs;
    int des_gray_val;

    // White Balance
    int wbp;
    double r_gain;
    double g_gain;
    double b_gain;

    // High Dynamic Range
    bool hdr;

    // Dark Current Filter
    int dcfm;

    // Pixel Clock
    int cpc;

    // Trigger Mode
    int ctm;

    // Trigger Source
    int cts;

    // Request
    int request;

    // Expose mode
    int cem;

    // Color Twist Input Correction Matrix Enable
    bool cticme;

    // Color Twist Input Correction Matrix Mode
    int cticmm;

    // Color Twist Enable
    bool cte;

    // Color Twist Row
    vector<double> ctrrow0;
    vector<double> ctrrow1;
    vector<double> ctrrow2;

    // Color Twist Output Correction Matrix Enable
    bool ctocme;

    // Color Twist Output Correction Matrix Mode
    int ctocmm;

    // LUT
    bool lute;
    int lutmode;
    int lutimpl;
    int lutmapping;

    // Camera Flash Mode
    int cfm;

    // Camera Flash Type
    int cft;
};

}