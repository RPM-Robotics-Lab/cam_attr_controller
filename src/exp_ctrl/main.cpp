#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <chrono>
#include <string>
#include<stdio.h> 
#include<stdlib.h>

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

    double et[256] = {-4.5241, -4.409, -4.2939, -4.1788, -4.0637, -3.9486, -3.8335, -3.7184, -3.6033, -3.4882, -3.3731, -3.2583, -3.1444, -3.0321, -2.9222, -2.8153, -2.7119, -2.6126, -2.5175, -2.4268, -2.3406, -2.2589, -2.1815, -2.1083, -2.0392, -1.9739, -1.9122, -1.8541, -1.7993, -1.7476, -1.6988, -1.6529, -1.6095, -1.5685, -1.5299, -1.4934, -1.4588, -1.4259, -1.3945, -1.3645, -1.3357, -1.3079, -1.281, -1.2549, -1.2296, -1.2049, -1.1807, -1.1571, -1.134, -1.1112, -1.0888, -1.0668, -1.0451, -1.0237, -1.0026, -0.9818, -0.96132, -0.94112, -0.92121, -0.9016, -0.88228, -0.86327, -0.84457, -0.82619, -0.80814, -0.7904, -0.77297, -0.75584, -0.73899, -0.72241, -0.70607, -0.68996, -0.67405, -0.65833, -0.64279, -0.62742, -0.61222, -0.59719, -0.58232, -0.56762, -0.55307, -0.53867, -0.52442, -0.51031, -0.49634, -0.48251, -0.4688, -0.45521, -0.44175, -0.42841, -0.41519, -0.40208, -0.38911, -0.37625, -0.36352, -0.35091, -0.33841, -0.32604, -0.31378, -0.30164, -0.28962, -0.27771, -0.26593, -0.25425, -0.24269, -0.23123, -0.21989, -0.20865, -0.19752, -0.1865, -0.1756, -0.16482, -0.15417, -0.14364, -0.13325, -0.12299, -0.11286, -0.10286, -0.092974, -0.083214, -0.073569, -0.064034, -0.054605, -0.045278, -0.036048, -0.026911, -0.017861, -0.008892, -1.3186e-10, 0.0088179, 0.017559, 0.026222, 0.03481, 0.043329, 0.051781, 0.060173, 0.068511, 0.076796, 0.08503, 0.093215, 0.10135, 0.10943, 0.11746, 0.12544, 0.13335, 0.1412, 0.14899, 0.1567, 0.16434, 0.17191, 0.1794, 0.18683, 0.19418, 0.20147, 0.2087, 0.21585, 0.22294, 0.22996, 0.23693, 0.24384, 0.2507, 0.25751, 0.26428, 0.271, 0.27767, 0.28431, 0.29089, 0.29744, 0.30395, 0.31043, 0.31688, 0.3233, 0.32969, 0.33605, 0.34239, 0.34869, 0.35496, 0.36117, 0.36734, 0.37344, 0.37949, 0.38549, 0.39143, 0.39732, 0.40317, 0.40897, 0.41472, 0.42044, 0.42613, 0.43181, 0.43747, 0.44312, 0.44876, 0.45439, 0.45999, 0.46558, 0.47115, 0.4767, 0.48222, 0.48773, 0.49321, 0.49868, 0.50413, 0.50957, 0.51498, 0.52038, 0.52576, 0.53112, 0.53647, 0.5418, 0.54713, 0.55244, 0.55773, 0.56299, 0.56822, 0.5734, 0.57855, 0.58364, 0.58867, 0.59364, 0.59855, 0.6034, 0.60818, 0.61291, 0.61759, 0.62221, 0.62678, 0.63131, 0.63579, 0.64022, 0.64464, 0.64902, 0.65338, 0.65772, 0.66202, 0.6663, 0.67057, 0.67483, 0.67909, 0.68334, 0.68759, 0.69184, 0.69609, 0.70033, 0.70457, 0.70881, 0.71304, 0.71727, 0.7215, 0.72572, 0.72995, 0.73418, 0.73842, 0.74265, 0.74688, 1.4093}; 

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
real_grab_and_return_ewg (bluefox2::Bluefox2 &cam_bluefox2, Img_eval &eval, double exp_t, double gain_t)
{
//    int64_t t0 = timestamp_now ();
    bot_core::image_t test_img;
    // grab image from the next_exp
    int exp_time = exp_t;
    double gain_db = gain_t;
    int tmp;

//    int64_t tt0 = timestamp_now();
//    while(1) {
        cam_bluefox2.SetExposeUs (exp_time);
        tmp = cam_bluefox2.GetExposeUs();
        cam_bluefox2.SetGainDB(gain_db);  // gain (-1db ~ 12 db )
        cam_bluefox2.set_timeout_ms (50);
        cam_bluefox2.RequestSingle();
        cam_bluefox2.GrabImage (test_img);
//        cerr << exp_time << "!=" << tmp << endl;
//        if (exp_time == tmp)    break;
//    }
    cv::Mat img;
    bot_util::botimage_to_cvMat (&test_img, img);

//    t1 = timestamp_now ();
    // compute entropy + grad

    double ewg = eval.calc_img_ent_grad (img, true);

    cv::Mat orignal_img = img;
    best=img;
//    double snr = eval.getPSNR (img, img); 
//    Mat noise_img = img; 
//    snr = eval.getPSNR (img, noise_img); 
    return ewg;
}

double
syn_grab_and_return_ewg (cv::Mat &synth_img_t, Img_eval &eval, double exp_t, double gain_t)
{
    double ewg = eval.calc_img_ent_grad (synth_img_t, true);

    return ewg;
}




double 
_synth_img_t(bluefox2::Bluefox2 &cam_bluefox2, double &next_exp, cv::Mat &synth_img_t, bool visualize)
//_synth_img_t(cv::Mat &synth_img_t, double &next_exp, cv::Mat &synth_img_t, bool visualize)

{
       // CRF from txt file TOBE delete 
    cv::Mat init_img;
    bot_core::image_t syn_test_img;

    // grab image from the next_exp
    int init_time = 1000;
    double init_db = 0.0;
    int tmp;

//    while(1) {

        cam_bluefox2.SetExposeUs (init_time);
        tmp = cam_bluefox2.GetExposeUs();
        cam_bluefox2.SetGainDB(init_db);  // gain (-1db ~ 12 db )
        cam_bluefox2.set_timeout_ms (50);
        cam_bluefox2.RequestSingle();
int64_t tt1 = timestamp_now();
        cam_bluefox2.GrabImage (syn_test_img);
int64_t tt2 = timestamp_now();
        //cout << "[ExpCtrl]\tOne loop done!! Next best exposure" << best_exposure << endl;
        double diff = static_cast<double>(tt2-tt1) / 1e6;
        printf ("Grab\tTime %f.\n", diff);


//        if (init_time == tmp)    break;
//    }
    bot_util::botimage_to_cvMat (&syn_test_img, init_img);
    cvtColor(init_img, init_img, cv::COLOR_BGR2GRAY);
//    cv::resize (img, img, cv::Size(320, 240));
//    next_exp = 1000.0; //from gpo
    int E = 20; 
    double init_irr = log( E * (0.00081));  // initial et = 1ms
    double intensity_ratio = 1.0;
    int A;
    for(int i = 0; i < 255; i++){
        if(et[i] > init_irr) {
            A = i;
            break;
        }
    }
    int exp_step;
    int exp_itv = 500;  // exposure interval 
    exp_step = ((int)next_exp - (int)init_time) / exp_itv + 1; //exptime = 1000 + (i-1)*500;
    std::cout<< "next exp_step for synth_t_img is  " << next_exp  << std::endl;

    double syn_n_exp = init_irr + log(exp_step);

    for(int k = 0; k < 255; k++){
        if(et[k] > syn_n_exp){ 
            int B = k;  // 6~~
            intensity_ratio = (double)B / (double)A;
            synth_img_t = init_img * intensity_ratio;   // Synth img along exposure time
            std::cout<< "next intensity_ratio for synth_t_img is  " << intensity_ratio << std::endl;
            if (visualize) {
                    // show text 
                CvFont font;
                cvInitFont (&font, CV_FONT_HERSHEY_PLAIN, 2.0, 2.0, 0, 2, 8);
                CvScalar red = CV_RGB (255, 0, 0);
                CvPoint str_pos = cvPoint (50, 50);
                char str[30];
                snprintf (str, sizeof str, "s_t%d", 1000+ exp_step*500 );
                cv::Mat result;
                cv::cvtColor(synth_img_t, result, cv::COLOR_GRAY2BGR);
                cv::resize (result, result, cv::Size(320, 240));
                cv::namedWindow("synth_img_t", cv::WINDOW_AUTOSIZE);
                cv::putText(result, str, Point(600,50), CV_FONT_HERSHEY_SIMPLEX, 1, CV_RGB(0,255,0), 2, 8);
                cv::imshow("synth_img_t", result);
                cv::imshow("init img", init_img);
                        cv::waitKey(10);

            }           
        break;
        }
    }
}

double 
_synth_img_g(cv::Mat &synth_img_t, double &next_gain, cv::Mat &synth_img_g, bool visualize)
{
    double init_db = next_gain;
    double g_factor, gain_step;
    gain_step = init_db/20; 
    g_factor = pow(7.01, gain_step); //factor = 7^((ii_g-1)/20); 
    synth_img_g = synth_img_t * g_factor; // Synth img along gain
    if (visualize) {
        CvFont font;
        cvInitFont (&font, CV_FONT_HERSHEY_PLAIN, 2.0, 2.0, 0, 2, 8);
        CvScalar red = CV_RGB (255, 0, 0);
        CvPoint str_pos = cvPoint (50, 50);
        char str[30];
        snprintf (str, sizeof str, "syn_gain%d", (int)init_db );
        cv::Mat result;
        cv::cvtColor(synth_img_g, result, cv::COLOR_GRAY2BGR);
        cv::resize (result, result, cv::Size(320, 240));
        cv::namedWindow("synth_img_g", cv::WINDOW_AUTOSIZE);
        cv::putText(result, str, Point(600,50), CV_FONT_HERSHEY_SIMPLEX, 1, CV_RGB(0,255,0), 2, 8);
        cv::imshow("synth_img_g", result);
//        cv::waitKey(10);
    }           
}

int
main(int argc, char *argv[])
{

    Bluefox2Parser parser;
    Img_eval eval;
    GPOptimize gpo;

    // init
    int init_expose = 1000;  // us
    double init_gain = 0.0;
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

//    int tmp_exposure=10;
    cam_bluefox2.SetFPSandExposeTime (frameRate_Hz, init_expose);

    cam_bluefox2.Configure (config);
    cam_bluefox2.set_timeout_ms (timeout_ms);

    
//    cam_bluefox2.SetExposeUs(init_expose);
////    cam_bluefox2.SetGainDB(init_expose);  // gain 
//    cam_bluefox2.set_timeout_ms(50);    
//    cam_bluefox2.RequestSingle();
//    bot_core::image_t tmp_img;
//    cam_bluefox2.GrabImage(tmp_img);


    // to save images
    _prepare_save_dir ();
    
    // main process start
    printf ("[ExpCtrl]\tStart to grab images.\n");

    // GP
    // intialize time from 100us - 150000us (150 time indeces)
    Config cfg(ls, s_f, s_n, AcqType::MAXVAR, 5);
    vector<double> x_data;

    while(true) {
        for (int t=1000; t<10500; t+=500) 
            x_data.push_back (t);

        gpo.set_predict (x_data);   // query exposure range
        gpo.initialize(cfg);

        // exp value is int but GP use double in general
        int next_exp = init_expose;
        double next_gain = init_gain;
        int best_exposure = 0;
        double best_gain = 0.0;
        double ewg = 0.0;
        double next_synth_index = 0.0;
        ewg = real_grab_and_return_ewg (cam_bluefox2, eval, next_exp, next_gain);
        cv::Mat control_img;
        cv::Mat synth_img_t, synth_img_g;
        int64_t time1 = timestamp_now();
        while (!gpo.is_optimal()) {
            cv::resize (ewg, control_img, cv::Size(320, 240));
    //            std::cout << "[ExpCtrl]\tDuring GP (t,v) = (" << next_exp << ", "<< ewg << ")" << std::endl;


            if (gpo.evaluate (next_exp, ewg)) {

                best_exposure = gpo.optimal_expose();
                best_gain = gpo.optimal_gain();

//        	    cv::namedWindow("best", cv::WINDOW_AUTOSIZE);
//	            cv::imshow("best", best);
    
//                cv::waitKey(0);
std::cout<< "exp time of optimal ewg  " << next_exp<< endl;
cvtColor(synth_img_t, synth_img_t, cv::COLOR_GRAY2BGR);
                ewg = syn_grab_and_return_ewg (synth_img_t, eval, best_exposure, best_gain);

                break;
            }
            else {
                int next_index = gpo.query_index();
                next_exp = x_data[next_index];
                next_synth_index = (double)next_exp;
int64_t stime1 = timestamp_now();
_synth_img_t(cam_bluefox2 , next_synth_index, synth_img_t, true);
_synth_img_g(synth_img_t, next_gain, synth_img_g,  true);
int64_t stime2 = timestamp_now();
double diff = static_cast<double>(stime2-stime1) / 1e6;
printf ("synthetic\tTime %f.\n", diff);
cvtColor(synth_img_t, synth_img_t, cv::COLOR_GRAY2BGR);
//cv::resize (synth_img_t, synth_img_t, cv::Size(320, 240));
std::cout<< "exp time of query ewg  " << next_exp<< endl;
                ewg = syn_grab_and_return_ewg (synth_img_t, eval, next_exp, next_gain);



            }
        }
        int64_t time2 = timestamp_now();
        //cout << "[ExpCtrl]\tOne loop done!! Next best exposure" << best_exposure << endl;
        next_exp =  best_exposure;
        next_gain = (int) best_gain;
        double diff = static_cast<double>(time2-time1) / 1e6;
        printf ("ExpCtrl\tTime %f.\n", diff);
        printf ("ExpCtrl\tSet to %d.\n", (int)next_exp);

         //write ** images 
        cv::Mat best_img ;
        bot_core::image_t best_img_t;

        cam_bluefox2.SetExposeUs(next_exp);
        cam_bluefox2.GetExposeUs();
        cam_bluefox2.SetGainDB(next_gain);  // gain (-1db ~ 12 db )
        cam_bluefox2.set_timeout_ms (50);
        cam_bluefox2.RequestSingle();
        cam_bluefox2.GrabImage(best_img_t);
        bot_util::botimage_to_cvMat(&best_img_t, best_img);
        cvtColor(best_img, best_img, cv::COLOR_BGR2GRAY);
        // show text 
        CvFont font;
        cvInitFont (&font, CV_FONT_HERSHEY_PLAIN, 2.0, 2.0, 0, 2, 8);
        CvScalar red = CV_RGB (255, 0, 0);
        CvPoint str_pos = cvPoint (50, 50);
        char str[30];
        snprintf (str, sizeof str, "%d", (int)next_exp);
        cv::Mat result;
        cv::cvtColor(best_img, result, cv::COLOR_GRAY2BGR);
        cv::namedWindow("best", cv::WINDOW_AUTOSIZE);
        cv::putText(result, str, Point(600,50), CV_FONT_HERSHEY_SIMPLEX, 1, CV_RGB(0,255,0), 2, 8);
        cv::imshow("best", result);
        
        cv::waitKey(0);

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
