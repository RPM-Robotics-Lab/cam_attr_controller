#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <chrono>
#include <string>

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
#include "irp-common/timestamp.h"

#include "Parser.h"
#include "bot_util/BotUtils.h"
#include "irp_imgeval++/img_eval.h"
#include <math.h>

using namespace std;
using namespace mvIMPACT::acquire;
using namespace ipms_param;


cv::Mat best;

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

    int64_t t0 = timestamp_now ();

    bot_core::image_t test_img;

    // grab image from the next_exp
    int exp_time = exp_t;
    int tmp;

    int64_t tt0 = timestamp_now();
    while(1) {
        
        cam_bluefox2.SetExposeUs (exp_time);
        tmp = cam_bluefox2.GetExposeUs();
        cam_bluefox2.set_timeout_ms (50);
        cam_bluefox2.RequestSingle();
        cam_bluefox2.GrabImage (test_img);
//        cerr << exp_time << "!=" << tmp << endl;
        if (exp_time == tmp)    break;
    }
//    cerr << timestamp_now() - tt0 << endl;
    //bool ok = cam_bluefox2.SetExpEnsure ();
    //ok = cam_bluefox2.SetExpEnsure ();

    
//    int64_t t1 = timestamp_now ();

//    cout << "ensure dt = " << timestamp_now ()-t1 << endl;


//    ok = cam_bluefox2.SetExpEnsure ();
    


//    cout << "ensure dt= " << timestamp_now ()-t0 << endl;


//    std::cout << "\n[ExpCtrl]\tgetexpose = " << cam_bluefox2.GetExposeUs() << "  Setexpose= " << exp_t<<  "" << std::endl;

    cv::Mat img;
    bot_util::botimage_to_cvMat (&test_img, img);
//    t1 = timestamp_now ();
    // compute entropy + grad
    double ewg = eval.calc_img_ent_grad (img, true);



    cv::Mat orignal_img = img;
    best=img;

    
    char str[30];
    snprintf (str, sizeof str, "%d", tmp);

    cv::Mat result;
    cv::cvtColor(img, result, cv::COLOR_GRAY2BGR);
    cv::namedWindow("best", cv::WINDOW_AUTOSIZE);
    cv::putText(result, str, Point(600,50), CV_FONT_HERSHEY_SIMPLEX, 1, CV_RGB(0,255,0), 2, 8);

//    cv::namedWindow("current", cv::WINDOW_AUTOSIZE);
//    cv::imshow("current", result);


//    cout << "ewg dt= " << timestamp_now ()-t1 << endl;

    // cam_bluefox2.RequestSingle();
//    std::cout << "[ExpCtrl]\t (t,v) = (" << exp_t << ", "<< ewg << ")" << std::endl;
//    cv::waitKey(0);

//	    cv::namedWindow("Original", cv::WINDOW_AUTOSIZE);
//	    cv::imshow("Original", orignal_img);
//    
////    cv::waitKey(0);

//    string str_save_path("result_data");
//    path save_path (str_save_path);
//    if (!is_directory(save_path)) {
//        if(boost::filesystem::create_directory(save_path)) {
//            cerr << "[ExpCtrl]\t Make a directory" << endl;   
//        }        
//    }

//    string str_time = std::to_string(timestamp_now());
//    string fname = str_save_path + "/" + str_time + ".png";
//    cv::imwrite(fname, orignal_img);

 
//    cout << "---all dt= " << timestamp_now ()-t0 << endl;
    return ewg;

}

int
main(int argc, char *argv[])
{
    Bluefox2Parser parser;
    Img_eval eval;
    GPOptimize gpo;

    // init
    int init_expose = 2000;  // us
    int frameRate_Hz = 50;  // fps
    int timeout_ms = 50;

    double ls = 3000.0;
    double s_f = 35.0;
    double s_n = 10.0;

    // camera prepare
    bluefox2::Bluefox2 cam_bluefox2(parser.GetSerial());
    bluefox2::Bluefox2Config config = parser.GetConfig();

    cam_bluefox2.Configure (config);
    cam_bluefox2.set_timeout_ms (timeout_ms);

    int tmp_exposure=10;
    cam_bluefox2.SetFPSandExposeTime (frameRate_Hz, init_expose);

    cam_bluefox2.Configure (config);
    cam_bluefox2.set_timeout_ms (timeout_ms);

    
    cam_bluefox2.SetExposeUs(init_expose);
    cam_bluefox2.set_timeout_ms(50);    
    cam_bluefox2.RequestSingle();
    bot_core::image_t tmp_img;
    cam_bluefox2.GrabImage(tmp_img);

    // to save images
    _prepare_save_dir ();
    
    // main process start
    printf ("[ExpCtrl]\tStart to grab images.\n");

    // GP
    // intialize time from 100us - 150000us (150 time indeces)
    Config cfg(ls, s_f, s_n, AcqType::MAXVAR, 5);
    vector<double> x_data;

    while(true) {
        for (int t=1000; t<10500; t+=814) 
            x_data.push_back (t);

        gpo.set_predict (x_data);   // query exposure range
        gpo.initialize(cfg);

        // exp value is int but GP use double in general
        int next_exp = init_expose;
        double best_exposure = 0.0;
        double ewg = 0.0;

        ewg = _grab_and_return_ewg (cam_bluefox2, eval, next_exp);
        cv::Mat control_img;
        int64_t time1 = timestamp_now();
        while (!gpo.is_optimal()) {
            cv::resize (ewg, control_img, cv::Size(320, 240));
    //            std::cout << "[ExpCtrl]\tDuring GP (t,v) = (" << next_exp << ", "<< ewg << ")" << std::endl;

            if (gpo.evaluate (next_exp, ewg)) {
                
                best_exposure = gpo.optimal_expose();

//        	    cv::namedWindow("best", cv::WINDOW_AUTOSIZE);
//	            cv::imshow("best", best);
    
//                cv::waitKey(0);
                ewg = _grab_and_return_ewg (cam_bluefox2, eval, best_exposure);
                break;
            }
            else {
                int next_index = gpo.query_index();
                next_exp = x_data[next_index];
                ewg = _grab_and_return_ewg (cam_bluefox2, eval, next_exp);
            }
        }
        int64_t time2 = timestamp_now();
        //cout << "[ExpCtrl]\tOne loop done!! Next best exposure" << best_exposure << endl;
        next_exp = (int) best_exposure;
        double diff = static_cast<double>(time2-time1) / 1e6;
        printf ("ExpCtrl\tTime %f.\n", diff);
        printf ("ExpCtrl\tSet to %d.\n", next_exp);

         //write ** images 
        cv::Mat best_img = best ;

        // show text 
        CvFont font;
        cvInitFont (&font, CV_FONT_HERSHEY_PLAIN, 2.0, 2.0, 0, 2, 8);
        CvScalar red = CV_RGB (255, 0, 0);
        CvPoint str_pos = cvPoint (50, 50);


        char str[30];
        snprintf (str, sizeof str, "%d", next_exp);

        cv::Mat result;
        cv::cvtColor(best_img, result, cv::COLOR_GRAY2BGR);
        cv::namedWindow("best", cv::WINDOW_AUTOSIZE);
        cv::putText(result, str, Point(600,50), CV_FONT_HERSHEY_SIMPLEX, 1, CV_RGB(0,255,0), 2, 8);
        cv::imshow("best", result);
        
    //    cv::waitKey(0);

        string str_save_path("result_data");
        path save_path (str_save_path);
        if (!is_directory(save_path)) {
            if(boost::filesystem::create_directory(save_path)) {
                cerr << "[ExpCtrl]\t Make a directory" << endl;   
            }        
        }

        string str_time = std::to_string(timestamp_now());
        string fname1 = str_save_path + "/" + str_time + ".png";
        string fname2 = str_save_path + "2/" + str_time + ".png";

        cv::imwrite(fname1, best_img);
        cv::imwrite(fname2, result);

    } // main while
  
    return 0;
}
