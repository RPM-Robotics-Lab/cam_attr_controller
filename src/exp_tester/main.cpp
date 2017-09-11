#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <time.h>
#include <numeric>
#include <cmath>
#include <algorithm>
#include <vector>
#include "irp_imgeval++/img_eval.h"

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
    cv::Mat img; 
    Img_eval eval;
    // Load image
    // src = cv::imread(argv[1], 1);
    img = cv::imread ("../../data/1.png", 1);
	Mat resized;
	cv::resize (img, img, cv::Size(320, 240));
    if (!img.data) {
        std::cout << "Usage: EntropyFilter <path_to_image>" << std::endl;
        return -1;
    }

    double ewg = eval.calc_img_ent_grad (img, true);

    char str[30];
    snprintf (str, sizeof str, "%d", 100);

    cv::namedWindow("best", cv::WINDOW_AUTOSIZE);

    cv::Mat result;
    cv::cvtColor(img, result, cv::COLOR_GRAY2BGR);
    cv::putText(result, str, Point(100,50), CV_FONT_HERSHEY_SIMPLEX, 1, CV_RGB(255,0,0), 2, 8);
    cv::imshow("best", result);

    cv::waitKey(0);

    std::cout << "Computed entropy weighted gain = " << ewg << std::endl;

    return 0;
}


