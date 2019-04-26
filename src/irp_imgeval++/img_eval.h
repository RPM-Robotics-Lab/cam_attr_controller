#pragma once
#include <string>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <time.h>
#include <numeric>
#include <cmath>
#include <algorithm>
#include <vector>

using namespace std;
using namespace cv;

class Img_eval {

public: 

    //Img_eval::Img_eval ();
    //Img_eval::~Img_eval ();
    void CreatErf(vector<vector<double> > &res1);
    void PrintErf(const vector<vector<double> > &res1); 
    void CreatGrf(vector<vector<double> > &res2);
    void PrintGrf(const vector<vector<double> > &res2); 
    double calc_img_ent_grad (cv::Mat &img, bool visualize = false);
    double getPSNR(const Mat &I1, const Mat &I2);
    double syn_grab_and_return_snr (cv::Mat &synth_img_g,  cv::Mat &synth_img_t, Img_eval &eval, double exp_t, double gain_t);
    void _synth_img_t (cv::Mat &init_img, double &next_exp, cv::Mat &synth_img_t, bool visualize);
    void _synth_img_g (cv::Mat &synth_img_t, double &next_gain, cv::Mat &synth_img_g, bool visualize);
    double syn_grab_and_return_ewg (cv::Mat &synth_img_g, Img_eval &eval, double exp_t, double gain_t);
private:
    void img_entropy (Mat &img, Mat &entropy);
    void img_wmask (Mat &entropy, Mat &wmask);
    void img_grad (Mat &img, Mat &grad);
    void img_Gmean(Mat &grad, double &Gmean);
    void img_Emean(Mat &entropy, double &Emean);
    void img_columnSum (Mat &entropy, Mat &columnSum, Mat &mu);
    void img_Gours (Mat &Gour, Mat &Gourstmp1, Mat &Gourstmp2, double &Gours);
	int sub_to_ind(int *coords, int *cumprod, int num_dims);
	void ind_to_sub(int p, int num_dims, const int size[],
    int *cumprod, int *coords);
	void getLocalEntropyImage(cv::Mat &gray, cv::Rect &roi, cv::Mat &entropy);
	void GammaCorrection(Mat& src, Mat& dst, float fGamma);



};
