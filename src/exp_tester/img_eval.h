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

    void img_entropy (Mat &img, Mat &entropy);
    void img_wmask (Mat &entropy, Mat &wmask);
    void img_grad (Mat &img, Mat &grad);
    void img_Gmean(Mat &grad, double &Gmean);
    void img_Emean(Mat &entropy, double &Emean);
    void img_columnSum (Mat &entropy, Mat &columnSum, Mat &mu);
    void img_Gours (Mat &Gour, Mat &Gourstmp, Mat &Gours);
	int sub_to_ind(int *coords, int *cumprod, int num_dims);
	void ind_to_sub(int p, int num_dims, const int size[],
    int *cumprod, int *coords);
	void getLocalEntropyImage(cv::Mat &gray, cv::Rect &roi, cv::Mat &entropy);
	void GammaCorrection(Mat& src, Mat& dst, float fGamma);
private:

};
