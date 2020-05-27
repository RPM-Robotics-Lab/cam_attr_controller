#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <chrono>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <boost/filesystem.hpp>

#include <irp_gpgo++/gp_optimize.h>
#include <irp_gpgo++/config.h>
#include "irp-common/timestamp.h"
#include "irp_imgeval++/img_eval.h"

#include "Bluefox2Driver/bluefox2.h"
#include "Bluefox2Driver/Bluefox2Config.h"
#include "param_util.h"
#include "lcmtypes++/bot_core/image_sync_t.hpp"
#include "Parser.h"
#include "bot_util/BotUtils.h"

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
    std::ofstream outFile;
    string str_times("response_data/times.txt");
    outFile.open(str_times.c_str());
}

void load_csv_from_file (std::ifstream& file_path, vector<VectorXd>& x_data) {
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
    std::ifstream file_path("../../data/exp_gain_table_index3.csv");
    Bluefox2Parser parser;
    Img_eval eval;
    GPOptimize gpo;
    string str_times("result_data/times.txt");
    string str_metric_times("result_data/metric_times.txt");
    string str_total_times("result_data/total_times.txt");

    std::ofstream outFile, outFile2, outFile3;
    outFile.open(str_times.c_str());
    outFile2.open(str_metric_times.c_str());
    outFile3.open(str_total_times.c_str());
    // init
    int init_expose ;  // 1000us
    double init_gain;
    int frameRate_Hz = 50;  // fps

    double ls = 15.5; // var: 15.5;
    double s_f = 4.85; // var: 15.0
    double s_n = 15.0;// var: 15.0
    int num_iter = 5;// var: 20

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
    double ewg;
    double ewg_opt;
    double ewg_real;
    while(true) {
        gpo.initialize(cfg);
        double next_synth_index_t = 0.0;  // 0 ~ 20000 us
        double best_synth_index_t = 0.0;
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
                best_synth_index_t = (double)best_exposure;
                best_gain = gpo.optimal_gain() -1;
                int64_t ctrl_diff = (timestamp_now()-time1) ;
                double hz = 1/ ((double)ctrl_diff/1000000);
//                std::cout<< ctrl_diff<< "[us], " << hz << "[hz] "<< endl;
                eval._synth_img_t (init_img, best_synth_index_t, synth_img_t, false);
                eval._synth_img_g (synth_img_t, best_gain, synth_img_g,  true);


                cvtColor(synth_img_g, synth_img_g, cv::COLOR_GRAY2BGR);
                ewg_opt = eval.syn_grab_and_return_ewg (synth_img_g, eval, best_exposure, best_gain);
//                cout << "Best = " << best_synth_index_t <<", " << best_gain  << ", metric = " << ewg_opt << endl;
                break;
            }

////selective
//            else if ( 0.7 < ((ewg_real/50)/ ewg_opt)  && ((ewg_real/50)/ ewg_opt) < 1.4)  {
//                cout << "ratio = " << ewg_opt/ (ewg_real/50) << endl;
//                break;
//            }
////selective//


            else {

            int64_t time_ctrl = timestamp_now();
                int next_index = gpo.query_index();
                x = x_data[next_index];
                next_exp = x_data[next_index](0)  * exp_itv;
                next_gain= x_data[next_index](1)-1;
                next_synth_index_t = (double)next_exp ;
                // synthetic image for exposure time and gain
            int64_t timesyn = timestamp_now();
                eval._synth_img_t (init_img, next_synth_index_t, synth_img_t, false);
                eval._synth_img_g (synth_img_t, next_gain, synth_img_g,  false);

            int64_t synth_diff = (timestamp_now()-timesyn) ;
            double synhz = 1/ ((double)synth_diff/1000000);

            int64_t timeeval = timestamp_now();
            cout << "synthetic time = " << synth_diff << "us" << ", " <<  synhz << "hz" <<endl;
                cvtColor(synth_img_g, synth_img_g, cv::COLOR_GRAY2BGR);
                ewg = eval.syn_grab_and_return_ewg (synth_img_g, eval, next_exp, next_gain);
            int64_t eval_diff = timestamp_now() - timeeval;
            double evalhz =  1/ ((double)eval_diff/1000000);
            cout << "eval time = " << eval_diff << "us , " << evalhz <<"hz "<< endl;


                outFile2  << synth_diff << " " << eval_diff << endl;
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


            int64_t ctrl_diff2 = (timestamp_now()-time1) ;
            double hz = 1/ ((double)ctrl_diff2/1000000);
            std::cout<< "-------------Total ctrl time = " << ctrl_diff2<< "[us], " << hz << "[hz] "<< endl;
                outFile3  << " " << ctrl_diff2 << " " << hz << " "  << endl;

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

            ewg_real = eval.calc_img_ent_grad (result , false);

//            std::cout << "real ewg_= " << ewg_real / 50 <<  std::endl;
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
    outFile2.close();
    outFile3.close();
    return 0;
}
