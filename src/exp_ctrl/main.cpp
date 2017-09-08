#include <unistd.h>
#include <iostream>

#include <boost/program_options/parsers.hpp>
#include <boost/filesystem.hpp>

#include <gp_optimize/gp_optimize.h>
#include <gp_optimize/config.h>

using namespace boost::program_options;
using namespace boost::filesystem;

#include "Bluefox2Driver/bluefox2.h"
#include "Bluefox2Driver/Bluefox2Config.h"
#include "param_util.h"
#include "lcmtypes++/bot_core/image_sync_t.hpp"

#include "Parser.h"
#include "bot_util/BotUtils.h"
#include "irp_imgeval++/img_eval.h"
#include <math.h>
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
    

    bot_core::image_t test_img;
    cam_bluefox2.GrabImage (test_img);

    cv::Mat img;
    bot_util::botimage_to_cvMat(&test_img, img);
    cv::resize (img, img, cv::Size(320, 240));

    // compute entropy + grad
    double ewg = eval.calc_img_ent_grad (img, true);

    cam_bluefox2.RequestSingle();

    cv::namedWindow("Current", cv::WINDOW_AUTOSIZE);
    cv::imshow("Current", img);


    return ewg;
}

int
main(int argc, char *argv[])
{

    Bluefox2Parser parser;
    Img_eval eval;
    GPOptimize gpo;

    // init
    int init_expose = 500;  // us
    int frameRate_Hz = 10;  // fps
    int timeout_ms = 200;

    double ls = 1000.0;
    double s_f = 1000.0;
    double s_n = 100.0;

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

    // GP
    // intialize time from 100us - 150000us (150 time indeces)
    Config cfg(ls, s_f, s_n, AcqType::MAXMI, 5);
    vector<double> x_data;
    for (int t=100; t<10000; t+=100) x_data.push_back (t);
    gpo.set_predict (x_data);   // query exposure range
    gpo.initialize(cfg);

    // exp value is int but GP use double in general
    int next_exp = init_expose;
    double best_exposure = 0.0;
    double ewg = 0.0;
    ewg = _grab_and_return_ewg (cam_bluefox2, eval, next_exp);

    //while(1) {
//    for (int i=0; i<10; i++) {
//        std::cout << "[ExpCtrl]\t(t,v) = (" << next_exp << ", "<< ewg << ")" << std::endl;
        while (!gpo.is_optimal()) {
            std::cout << "[ExpCtrl]\tDuring GP (t,v) = (" << next_exp << ", "<< ewg << ")" << std::endl;
            if (gpo.evaluate (next_exp, ewg)) {
                best_exposure = gpo.optimal_expose();
                break;
            }
            else {
                int next_index = gpo.query_index();
                next_exp = x_data[next_index];
                ewg = _grab_and_return_ewg (cam_bluefox2, eval, next_exp);
            }
        }

        //cout << "[ExpCtrl]\tOne loop done!! Next best exposure" << best_exposure << endl;
        next_exp = (int) best_exposure;

        printf ("ExpCtrl\tSet to %d.\n", next_exp);
        cv::waitKey(0);


//    }

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
