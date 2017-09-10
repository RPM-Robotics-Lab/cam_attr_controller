#include <unistd.h>
#include <iostream>

#include <boost/program_options/parsers.hpp>
#include <boost/filesystem.hpp>


using namespace boost::program_options;
using namespace boost::filesystem;

#include "Bluefox2Driver/bluefox2.h"
#include "Bluefox2Driver/Bluefox2Config.h"
#include "param_util.h"
#include "lcmtypes++/bot_core/image_sync_t.hpp"

#include "Parser.h"
#include "bot_util/BotUtils.h"
#include "irp_imgeval++/img_eval.h"

using namespace std;
using namespace mvIMPACT::acquire;
using namespace ipms_param;

void
_prepare_save_dir ()
{
    string str_save_path("response_data");
    path save_path (str_save_path);
    if (!is_directory(save_path)) {
        if(boost::filesystem::create_directory(save_path)) {
            cerr << "[ExpCtrl]\t Make a directory" << endl;   
        }        
    }
    string str_save_img_path("response_data/images");
    path save_img_path(str_save_img_path);
    if (!is_directory(save_img_path)) {
        if(boost::filesystem::create_directory(save_img_path)) {
            cerr << "[ExpCtrl]\t Make a directory" << endl;   
        }        
    }
    ofstream outFile;
    string str_times("response_data/times.txt");
    outFile.open(str_times.c_str());
}

double
_grab_and_return_ewg (bluefox2::Bluefox2 &cam_bluefox2, Img_eval &eval, int exp_t)
{
    // grab image from the next_exp
    cam_bluefox2.SetExposeUs (exp_t);
    cam_bluefox2.RequestSingle();

    bot_core::image_t test_img;
    cam_bluefox2.GrabImage (test_img);

    cv::Mat img;
    bot_util::botimage_to_cvMat(&test_img, img);
    cv::resize (img, img, cv::Size(320, 240));

    // compute entropy + grad
    double ewg = eval.calc_img_ent_grad (img, false);


    cv::namedWindow("Previous", cv::WINDOW_AUTOSIZE);
    cv::imshow("Previous", img);
//    cv::waitKey(0);

    return ewg;
}

int
main(int argc, char *argv[])
{

    Bluefox2Parser parser;
    Img_eval eval;

    // init
    int init_expose = 300;  // us
    int frameRate_Hz = 10;  // fps
    int timeout_ms = 200;

    // camera prepare
    bluefox2::Bluefox2 cam_bluefox2(parser.GetSerial());
    bluefox2::Bluefox2Config config = parser.GetConfig();

    cam_bluefox2.Configure (config);
    cam_bluefox2.set_timeout_ms (timeout_ms);
    cam_bluefox2.SetFPSandExposeTime (frameRate_Hz, init_expose);

    // to save images
    _prepare_save_dir ();
    
    // main process start
    printf ("[ExpCtrl]\tStart to grab images.\n");

    for (int t=500; t<20000; t+=500) {
	double ewg = _grab_and_return_ewg (cam_bluefox2, eval, t);
//        std::cout << "[ExpCtrl]\t(t,v) = (" << t << ", "<< ewg << ")" << std::endl;
        std::cout << t << "," << ewg  << std::endl;
//        printf ("%d, %g\n", t,ewg);
    }

    //vector<int> compression_params;
    //compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
    //compression_params.push_back(0);

    //string save_img_name = str_save_img_path + "/" + str_num_data + ".png";
    //cv::imwrite(save_img_name, gray, compression_params);

    /*cam_bluefox2.SetExposeUs(init_expose);
    cam_bluefox2.set_timeout_ms(200);    
    cam_bluefox2.RequestSingle();
    bot_core::image_t tmp_img;
    cam_bluefox2.GrabImage(tmp_img);*/


    return 0;
}
