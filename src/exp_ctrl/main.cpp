#include <unistd.h>
#include <iostream>
#include <lcm/lcm-cpp.hpp>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
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

int
main(int argc, char *argv[])
{

    options_description bluefox2opt("Bluefox2 Driver");
    variables_map vm;

    try {
        
        bluefox2opt.add_options()
            ("help,h","produce a help message");
            // ("side,s",value<string>()->default_value("left"),
            //     "The side of stereo-camera : left or right");

        store(parse_command_line(argc, argv, bluefox2opt), vm);

        if(vm.count("help")) {
            cout << bluefox2opt;
        }
        // else if(vm.count("side")) {
        //     cout << "This camera is the \"" << vm["side"].as<string>() << "\" side of the stereo camera." <<endl;
        // }

    }
    catch (exception& e) {

        cout << bluefox2opt << endl;
        cout << e.what() << endl;
        cout << "error!!" << endl;
        return 0;
    }  

    // string cam_side = vm["side"].as<string>();     
    Bluefox2Parser parser;

    int exposureTime_us = 10;
    int frameRate_Hz = 10;

    bluefox2::Bluefox2 cam_bluefox2(parser.GetSerial());

    bluefox2::Bluefox2Config config = parser.GetConfig();
    cam_bluefox2.Configure(config);
    cam_bluefox2.set_timeout_ms(200);

    cam_bluefox2.SetFPSandExposeTime(frameRate_Hz, exposureTime_us);

    cam_bluefox2.Configure(config);
    cam_bluefox2.set_timeout_ms(200);

    int init_expose = 10;
    cam_bluefox2.SetExposeUs(init_expose);
    cam_bluefox2.set_timeout_ms(200);    
    cam_bluefox2.RequestSingle();
    bot_core::image_t tmp_img;
    cam_bluefox2.GrabImage(tmp_img);

    string str_save_path("response_data");
    path save_path(str_save_path);

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
    

    printf ("ExpCtrl\tStart to grab images.\n");

    Img_eval eval;

    //while(1) {
    for (int i=0; i<50; i++) {
        // grab image
        bot_core::image_t test_img;
        cam_bluefox2.GrabImage (test_img);

        cv::Mat img;
        bot_util::botimage_to_cvMat(&test_img, img);

        // compute entropy + grad
        double ewg = eval.calc_img_ent_grad (img, false);

        std::cout << "Computed entropy weighted gain = " << ewg << std::endl;

        // GP exp compute
        // TODO
        int next_exp = 3500;

        for (int j=0; j<10; j++) {
            next_exp += i * 20;
        }

        // assign the new exp time
        cam_bluefox2.SetExposeUs(next_exp);
        //cam_bluefox2.set_timeout_ms(100);
        // usleep(200000);

        cam_bluefox2.RequestSingle();
        printf ("ExpCtrl\tSet to %d.\n", next_exp);
        //vector<int> compression_params;
        //compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
        //compression_params.push_back(0);

        //string save_img_name = str_save_img_path + "/" + str_num_data + ".png";
        //cv::imwrite(save_img_name, gray, compression_params);
    }




    return 0;
}
