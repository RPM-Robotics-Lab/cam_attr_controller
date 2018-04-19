#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include<fstream>
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

    // CRF from txt file 
    double et[256] = {-4.5241, -4.409, -4.2939, -4.1788, -4.0637, -3.9486, -3.8335, -3.7184, -3.6033, -3.4882, -3.3731, -3.2583, -3.1444, -3.0321, -2.9222, -2.8153, -2.7119, -2.6126, -2.5175, -2.4268, -2.3406, -2.2589, -2.1815, -2.1083, -2.0392, -1.9739, -1.9122, -1.8541, -1.7993, -1.7476, -1.6988, -1.6529, -1.6095, -1.5685, -1.5299, -1.4934, -1.4588, -1.4259, -1.3945, -1.3645, -1.3357, -1.3079, -1.281, -1.2549, -1.2296, -1.2049, -1.1807, -1.1571, -1.134, -1.1112, -1.0888, -1.0668, -1.0451, -1.0237, -1.0026, -0.9818, -0.96132, -0.94112, -0.92121, -0.9016, -0.88228, -0.86327, -0.84457, -0.82619, -0.80814, -0.7904, -0.77297, -0.75584, -0.73899, -0.72241, -0.70607, -0.68996, -0.67405, -0.65833, -0.64279, -0.62742, -0.61222, -0.59719, -0.58232, -0.56762, -0.55307, -0.53867, -0.52442, -0.51031, -0.49634, -0.48251, -0.4688, -0.45521, -0.44175, -0.42841, -0.41519, -0.40208, -0.38911, -0.37625, -0.36352, -0.35091, -0.33841, -0.32604, -0.31378, -0.30164, -0.28962, -0.27771, -0.26593, -0.25425, -0.24269, -0.23123, -0.21989, -0.20865, -0.19752, -0.1865, -0.1756, -0.16482, -0.15417, -0.14364, -0.13325, -0.12299, -0.11286, -0.10286, -0.092974, -0.083214, -0.073569, -0.064034, -0.054605, -0.045278, -0.036048, -0.026911, -0.017861, -0.008892, -1.3186e-10, 0.0088179, 0.017559, 0.026222, 0.03481, 0.043329, 0.051781, 0.060173, 0.068511, 0.076796, 0.08503, 0.093215, 0.10135, 0.10943, 0.11746, 0.12544, 0.13335, 0.1412, 0.14899, 0.1567, 0.16434, 0.17191, 0.1794, 0.18683, 0.19418, 0.20147, 0.2087, 0.21585, 0.22294, 0.22996, 0.23693, 0.24384, 0.2507, 0.25751, 0.26428, 0.271, 0.27767, 0.28431, 0.29089, 0.29744, 0.30395, 0.31043, 0.31688, 0.3233, 0.32969, 0.33605, 0.34239, 0.34869, 0.35496, 0.36117, 0.36734, 0.37344, 0.37949, 0.38549, 0.39143, 0.39732, 0.40317, 0.40897, 0.41472, 0.42044, 0.42613, 0.43181, 0.43747, 0.44312, 0.44876, 0.45439, 0.45999, 0.46558, 0.47115, 0.4767, 0.48222, 0.48773, 0.49321, 0.49868, 0.50413, 0.50957, 0.51498, 0.52038, 0.52576, 0.53112, 0.53647, 0.5418, 0.54713, 0.55244, 0.55773, 0.56299, 0.56822, 0.5734, 0.57855, 0.58364, 0.58867, 0.59364, 0.59855, 0.6034, 0.60818, 0.61291, 0.61759, 0.62221, 0.62678, 0.63131, 0.63579, 0.64022, 0.64464, 0.64902, 0.65338, 0.65772, 0.66202, 0.6663, 0.67057, 0.67483, 0.67909, 0.68334, 0.68759, 0.69184, 0.69609, 0.70033, 0.70457, 0.70881, 0.71304, 0.71727, 0.7215, 0.72572, 0.72995, 0.73418, 0.73842, 0.74265, 0.74688, 1.4093}; 

//    double gain[128] = {

//    cout << "rrrrrrr "<<  << endl;


    // Load image
    // src = cv::imread(argv[1], 1);
    img = cv::imread ("../../data/110_1.png", 1);
	Mat resized;
//	cv::resize (img, img, cv::Size(320, 240));
    if (!img.data) {
        std::cout << "Usage: EntropyFilter <path_to_image>" << std::endl;
        return -1;
    }

    double ewg = eval.calc_img_ent_grad (img, true);
    int exptime = 1000  ; 
    int E = 200; 
    double init_irr = log( E * (0.00005+ (1 * 0.00005)));  // initial et = 1ms
    double intensity_ratio = 1.0;

    int A;
    for (int i = 1; i<156; i++)   {
        for(int j = 0; j < 256; j++){
            if(et[j] > init_irr) {
            A = j;
            break;
            }
        }
        std::cout << "initial exposure time is " << A << ","  << et[A]  <<  std::endl;
        
        double n_exp = init_irr + log(i/4);
        
        for(int k = 0; k < 256; k++){
            if(et[k] > n_exp){ 
            int B = k;  // 6~~
            double k_int = B / A;
        
            std::cout << "Next intensity is " << B << ",  "  << et[B] << ",  " << k_int <<  std::endl;
            Mat etimg = img * k_int; 
        
            cv::imshow("etimg", etimg);
            cv::waitKey(10);
            cvtColor(etimg, etimg, cv::COLOR_GRAY2BGR);
            double etewg = eval.calc_img_ent_grad (etimg, true);
            std::cout << "Computed entropy weighted gain = " << etewg << std::endl;
            break;
            double snr = eval.getPSNR (img, img); 
            Mat noise_img = img; 
            snr = eval.getPSNR (img, noise_img); 
            cv::imshow("Noisy", noise_img);

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
        }
    }

}
//        for (int j = 0; j<127; j+=10)   {
//    //    cout << pow(10, gain[j]/5) << endl ;
//        Mat gainimg = etimg * pow(10, gain[j]/10); 
//        cv::imshow("gainimg", gainimg);
//        cv::waitKey(1);
//        cvtColor(gainimg, gainimg, cv::COLOR_GRAY2BGR);
//        double gainewg = eval.calc_img_ent_grad (gainimg, true);
//        std::cout << "Computed Gain ewg = " << gainewg << "irr" << irr << std::endl;  
//        }    
 


