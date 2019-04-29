#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <chrono>
#include <string>
#include <stdio.h> 
#include <stdlib.h>
#include <boost/filesystem.hpp>
#include <irp_gpgo++/gp_optimize.h>
#include <irp_gpgo++/config.h>
#include "param_util.h"
#include "irp_imgeval++/img_eval.h"
#include <math.h>
using namespace boost::filesystem;
using namespace std;
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

int main(int argc, char *argv[])
{
    ifstream file_path("../../data/exp_gain_table_index1.csv");
    Img_eval eval;
    GPOptimize gpo;
    string str_times("result_data/times.txt");
    ofstream outFile;
    outFile.open(str_times.c_str());
    // init
    int init_expose ;  // 1000us
    double init_gain;
    double ls = 15.5; // var: 15.5;
    double s_f = 4.85; // var: 15.0
    double s_n = 15.0;// var: 15.0
    int num_iter = 20;// var: 20
    bool vis_and_imwrite_best = true;
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
        double best_synth_index_t = 0.0;  // 0 ~ 20000 us
        double best_synth_index_g = 0.0;  // 0 ~ 12 db

        cv::Mat control_img;
        cv::Mat synth_img_t, synth_img_g;
        // camera grab for t0
        // image preparation
        cv::Mat init_img;
        init_img = cv::imread ("../../data/indoor/1_4.png", 1);
	    Mat resized;
        if (!init_img.data) {
            std::cout << "Error <path_to_image>" << std::endl;
        return -1;
        }
        cvtColor(init_img, init_img, cv::COLOR_BGR2GRAY);
     
       while (!gpo.is_optimal()) { // GPO WHILE
       
            if (gpo.evaluate (x, ewg)) {
                cv::Mat synth_img_t_best, synth_img_g_best;
                // Optimal attribute found. Break while loop.
                int best_index = gpo.optimal_index(); 
                best_exposure = t0 + x_data[best_index](0)  * exp_itv - exp_itv;
                best_gain= x_data[best_index](1)-1;
                best_synth_index_t = (double)best_exposure ;
                best_synth_index_g = (double)best_gain;
                cvtColor(synth_img_g, synth_img_g, cv::COLOR_GRAY2BGR);
                cout << "Best = " << best_exposure <<", " << best_gain << endl;
                eval._synth_img_t (init_img, best_synth_index_t, synth_img_t_best, false);
                eval._synth_img_g (synth_img_t_best, best_synth_index_g, synth_img_g_best,  false);
                cv::imshow("best", synth_img_g_best);
                cv::waitKey();
                break;
            }
            else {
                int next_index = gpo.query_index();
                x = x_data[next_index];
                next_exp = t0+ x_data[next_index](0)  * exp_itv - exp_itv;
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
            // assing the found optimal gain and exposure time
            int best_exposure_compensate_init = best_exposure ;  // + 500
            // prepare opencv for visualize
            // show text 
            CvFont font;
            cvInitFont (&font, CV_FONT_HERSHEY_PLAIN, 2.0, 2.0, 0, 2, 8);
            CvScalar red = CV_RGB (255, 0, 0);
            CvPoint str_pos = cvPoint (50, 50);
            char str[30];
            snprintf (str, sizeof str, "%d, %d", best_exposure_compensate_init , (int)best_gain);
            cv::Mat result;
            cv::namedWindow("best", cv::WINDOW_AUTOSIZE);
            cv::putText(result, str, Point(550,30), CV_FONT_HERSHEY_SIMPLEX, 1, CV_RGB(0,255,0), 2, 8);
            cv::resize (synth_img_g, synth_img_g, cv::Size(752, 480));
            cv::imshow("synthetic", synth_img_g);
            cv::waitKey();
            if (!is_directory(save_path)) {
                if(boost::filesystem::create_directory(save_path)) {
                }        
            }
        int num_data = 0;     
            for (int j=0; j<1; j++) {     // num capture img
                stringstream ss_num_data;
                ss_num_data << setw(5) << setfill('0') << num_data++;
                string str_num_data(ss_num_data.str());
            }

        }

    } // main while
    outFile.close();  
    return 0;
}



