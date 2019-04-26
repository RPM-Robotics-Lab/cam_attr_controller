#include <iostream>
#include <sstream>
#include <fstream>
#include "irp_imgeval++/img_eval.h"
#include <math.h>


using namespace std;

int main(int argc, char** argv)
{

    cv::Mat img; 
    Img_eval eval;
    img = cv::imread ("../../data/1_1.png", 1);
	Mat resized;
//	cv::resize (img, img, cv::Size(320, 240));
    if (!img.data) {
        std::cout << "Error <path_to_image>" << std::endl;
        return -1;
    }
    double ewg = eval.calc_img_ent_grad (img, true);
    cout << "ewg = " << ewg << endl;
}
