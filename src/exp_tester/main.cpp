#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <time.h>
#include <numeric>
#include <cmath>
#include <algorithm>
#include <vector>
#include "img_eval.h"
using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
    cv::Mat img; 
    Img_eval eval;
    // Load image
    // src = cv::imread(argv[1], 1);
    img = cv::imread ("../../data/62.png", 1);
	Mat resized;
	cv::resize (img, img, cv::Size(320, 240));
    if (!img.data) {
        std::cout << "Usage: EntropyFilter <path_to_image>" << std::endl;
        return -1;
    }

    // Convert to grayscale
    cvtColor(img, img, cv::COLOR_BGR2GRAY);

    cv::Mat entropy, grad ;
	cv::Mat wmask(entropy.size(), CV_32F, 1.); // ones
    eval.img_entropy (img, entropy);
    eval.img_wmask (entropy, wmask);
    eval.img_grad (img, grad);

	int wrows = entropy.rows;
	int wcols = entropy.cols; 
    double Gmean, Emean;
    eval.img_Gmean (grad, Gmean);
    eval.img_Emean (entropy, Emean);

	printf("Gradient sss is %d %d \n", wcols, wrows) ;
 	
	Mat gradW = grad > Emean * 0.3;
	gradW *= 1;
	imshow("gradW", gradW);
	
	Mat columnSum, mu;   
    eval.img_columnSum (entropy, columnSum, mu);
	int Sval = Gmean;  
	Mat Smask = Gmean * wmask;  //Smask == Sval, how to - value
    Mat Gour = (grad+Smask) / (wmask.cols * wmask.rows);
	Mat Gourstmp, Gours;
	eval.img_Gours (Gour, Gourstmp, Gours);

    std::cout << "Mean: " << mu.at<double>(0,0) << "   Sval: "<< Gours << std::endl;

    /// Display results
	cv::namedWindow("Original", cv::WINDOW_AUTOSIZE);
	cv::namedWindow("Entropy Mask", cv::WINDOW_AUTOSIZE);	
	cv::namedWindow("Entropy Filter", cv::WINDOW_AUTOSIZE);
	cv::namedWindow("Gradient Image", cv::WINDOW_AUTOSIZE);
	cv::imshow("Original", img);
	cv::imshow("Entropy Mask", wmask);  //dst 
	cv::imshow("Entropy Filter", entropy);
	cv::imshow("Gradient Image",grad);
    /// Wait until user exits the program
    cv::waitKey(0);

    return 0;
}


