#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <chrono>
#include <string>
#include<stdio.h> 
#include<stdlib.h>
#include <boost/filesystem.hpp>
#include <gp_optimize/gp_optimize.h>
#include <gp_optimize/config.h>
#include "Bluefox2Driver/bluefox2.h"
#include "Bluefox2Driver/Bluefox2Config.h"
#include "param_util.h"
#include "lcmtypes++/bot_core/image_sync_t.hpp"
#include "irp-common/timestamp.h"
#include "Parser.h"
#include "bot_util/BotUtils.h"
#include "irp_imgeval++/img_eval.h"
#include <math.h>
using namespace boost::filesystem;
using namespace std;
using namespace mvIMPACT::acquire;
using namespace ipms_param;

int t0 = 1000;      // initial expose time
double g0 = 0.0;    // 0.0 = initial gai
int exp_itv = 500;  // exposure interval 

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


void load_csv_from_file (ifstream& file_path, vector<VectorXd>& x_data) {
    string line;

    // Load data
    while(getline(file_path, line))
    {
        int index = 0;
        std::stringstream  lineStream(line);
        std::string        cell;
        double exposure, gain;
        while(std::getline(lineStream,cell,','))
        {

            if (index == 0){
                exposure = std::strtod(cell.c_str(),0);
            }
            else if (index == 1) {
                gain = std::strtod(cell.c_str(),0);
                VectorXd data(2);
                data << exposure, gain;
                x_data.push_back(data);
            }
            index ++;
        }
    }
}

int
main(int argc, char *argv[])
{
    ifstream file_path("../../data/exp_gain_table_index3.csv");
    Bluefox2Parser parser;
    Img_eval eval;
    GPOptimize gpo;

    string str_times("result_data/times.txt");
    ofstream outFile;
    outFile.open(str_times.c_str());
    // init
    int init_expose ;  // 1000us
    double init_gain;
    int frameRate_Hz = 50;  // fps

    double ls = 15.5; // var: 15.5;
    double s_f = 4.85; // var: 15.0
    double s_n = 15.0;// var: 15.0
    int num_iter = 20;// var: 20

    bool vis_and_imwrite_best = true;

    // camera prepare
    // --------------------------------------------//
    bluefox2::Bluefox2 cam_bluefox2(parser.GetSerial());
    bluefox2::Bluefox2Config config = parser.GetConfig();
    cam_bluefox2.Configure (config);
    cam_bluefox2.SetFPSandExposeTime (frameRate_Hz, init_expose);
    // take #2
    cam_bluefox2.SetExposeUs(init_expose);
    cam_bluefox2.SetGainDB(init_gain); 
    bot_core::image_t tmp_img;
    // --------------------------------------------//

    // GP
    Config cfg(ls, s_f, s_n, AcqType::MAXMI, 50, num_iter); //var : 0.5
    vector<double> x_data_t, x_data_g;
    vector<VectorXd> x_data;    
    load_csv_from_file(file_path, x_data);
    cout << "Data load done " << endl
         << "\t Data1: " << x_data.size() << endl;
    VectorXd x = x_data[0]; // current exposure (minimum exposure not good for initialize)
    int next_exp = init_expose;
    double next_gain = init_gain;    
    int best_exposure = x_data[0](0); // for safety
    double best_gain = x_data[0](1);
    // set predict once before while
    gpo.set_predict (x_data);   // query exposure range
    double ewg = 0.f;
    while(true) {
        gpo.initialize(cfg);
        double next_synth_index_t = 0.0;  // 0 ~ 20000 us
        double next_synth_index_g = 0.0;  // 0 ~ 12 db
        cv::Mat control_img;
        cv::Mat synth_img_t, synth_img_g;
        // camera grab for t0
        bot_core::image_t syn_test_img;
        cam_bluefox2.SetExposeUs(t0);
        cam_bluefox2.GetExposeUs();     // get is needed to confirm setexposure()
        cam_bluefox2.SetGainDB(g0);     // gain (-1db ~ 12 db )
        // cam_bluefox2.set_timeout_ms (50);
        cam_bluefox2.RequestSingle();
        cam_bluefox2.GrabImage (syn_test_img);
        // image preparation
        cv::Mat init_img;
        bot_util::botimage_to_cvMat (&syn_test_img, init_img);
        cvtColor(init_img, init_img, cv::COLOR_BGR2GRAY);
        // cv::resize (img, img, cv::Size(320, 240));
        int64_t time1 = timestamp_now();
     
       while (!gpo.is_optimal()) { // GPO WHILE
       
            if (gpo.evaluate (x, ewg)) {
                // Optimal attribute found. Break while loop.
                best_exposure = gpo.optimal_expose() * exp_itv ;
                best_gain = gpo.optimal_gain() -1;  
                int64_t ctrl_diff = (timestamp_now()-time1) ;
                double hz = 1/ ((double)ctrl_diff/1000000);
                std::cout<< ctrl_diff<< "[us], " << hz << "[hz] "<< endl;
                cvtColor(synth_img_g, synth_img_g, cv::COLOR_GRAY2BGR);
//                ewg = eval.syn_grab_and_return_ewg (synth_img_g, eval, best_exposure, best_gain);
                cout << "Best = " << best_exposure <<", " << best_gain << endl;
                break;
            }
            else {
                int next_index = gpo.query_index();
                x = x_data[next_index];
                next_exp = x_data[next_index](0)  * exp_itv;
                next_gain= x_data[next_index](1)-1;
                next_synth_index_t = (double)next_exp ;
                next_synth_index_g = (double)next_gain;
                // synthetic image for exposure time and gain
                eval._synth_img_t (init_img, next_synth_index_t, synth_img_t, false);
                eval._synth_img_g (synth_img_t, next_synth_index_g, synth_img_g,  false);
                cvtColor(synth_img_g, synth_img_g, cv::COLOR_GRAY2BGR);
                ewg = eval.syn_grab_and_return_ewg (synth_img_g, eval, next_exp, next_gain);
                std::cout << "next_= " << next_exp << ", " << next_gain <<", "<< ewg <<  std::endl;
            }

        } // GPO WHILE
    // result writing
    string str_save_path("result_data");
    path save_path (str_save_path);

        // optimal found!
        if (vis_and_imwrite_best) {
            cv::Mat best_img;
            // assing the found optimal gain and exposure time
            int best_exposure_compensate_init = best_exposure ;  // + 500
            cam_bluefox2.SetExposeUs(best_exposure_compensate_init);
            cam_bluefox2.GetExposeUs();
            cam_bluefox2.SetGainDB(best_gain);  // gain (-1db ~ 12 db )
            cam_bluefox2.RequestSingle();
            // prepare opencv for visualize
            bot_core::image_t best_img_t;
            cam_bluefox2.GrabImage(best_img_t);
            bot_util::botimage_to_cvMat(&best_img_t, best_img);
            cvtColor(best_img, best_img, cv::COLOR_BGR2GRAY);
            // show text 
            CvFont font;
            cvInitFont (&font, CV_FONT_HERSHEY_PLAIN, 2.0, 2.0, 0, 2, 8);
            CvScalar red = CV_RGB (255, 0, 0);
            CvPoint str_pos = cvPoint (50, 50);
            char str[30];
            snprintf (str, sizeof str, "%d, %d", best_exposure_compensate_init , (int)best_gain);
            cv::Mat result;
            cv::cvtColor(best_img, result, cv::COLOR_GRAY2BGR);
            cv::namedWindow("best", cv::WINDOW_AUTOSIZE);
            cv::putText(result, str, Point(550,30), CV_FONT_HERSHEY_SIMPLEX, 1, CV_RGB(0,255,0), 2, 8);
            cv::imshow("best", result);
            cv::waitKey(10);
            
            if (!is_directory(save_path)) {
                if(boost::filesystem::create_directory(save_path)) {
                }        
            }
        
        int num_data = 0;     
            for (int j=0; j<1; j++) {     // num capture img
                stringstream ss_num_data;
                ss_num_data << setw(5) << setfill('0') << num_data++;
                string str_num_data(ss_num_data.str());
                string str_time = std::to_string(timestamp_now());
                outFile << str_time <<  " " <<  (int)best_gain << " " << best_exposure_compensate_init << setprecision(5) << endl;
                string fname1 = str_save_path + "/" + str_time + ".png"; 
//                string fname2 = str_save_path + "/2/" + str_time + ".png";
                cv::imwrite(fname1, best_img);
//                cv::imwrite(fname2, result);

            }

        }
//        printf ("ExpCtrl\tSet to %d. %d \n", best_exposure, (int)best_gain);

    } // main while
    outFile.close();  
    return 0;
}



