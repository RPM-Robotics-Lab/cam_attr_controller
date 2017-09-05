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
#include "param-util/param_util.h"
#include "lcmtypes++/bot_core/image_sync_t.hpp"

#include "Parser.h"

#include "bot_util/BotUtils.h"

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
            cerr << "[CalibDAQ]\t Make a directory" << endl;   
        }        
    }

    string str_save_img_path("response_data/images");
    path save_img_path(str_save_img_path);
    if (!is_directory(save_img_path)) {
        if(boost::filesystem::create_directory(save_img_path)) {
            cerr << "[CalibDAQ]\t Make a directory" << endl;   
        }        
    }

    ofstream outFile;
    string str_times("response_data/times.txt");
    outFile.open(str_times.c_str());
    

    int num_data = 0;

    for (int expose_us = 10; expose_us <= 10000; expose_us += 50) {

        for (int j=0; j<1; j++) {     // num capture img
            cam_bluefox2.SetExposeUs(expose_us);
            cam_bluefox2.set_timeout_ms(100);
            // usleep(200000);

            cam_bluefox2.RequestSingle();
            
            bot_core::image_t test_img;
            bot_core::image_sync_t test_img_sync_t;
            cam_bluefox2.GrabImage(test_img);
            test_img_sync_t.utime = test_img.utime;

            stringstream ss_num_data;
            ss_num_data << setw(1) << setfill('0') << num_data++;
            string str_num_data(ss_num_data.str());

            outFile << str_num_data << " " << test_img.utime << " " << cam_bluefox2.GetExposeUs()/1000.0 << setprecision(5) << endl;

            cv::Mat img;
            bot_util::botimage_to_cvMat(&test_img, img);

            cv::Mat gray;
            if(img.type() != CV_8UC1) {
                cv::cvtColor(img,gray,cv::COLOR_BGR2GRAY);
            }

            vector<int> compression_params;
            compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
            compression_params.push_back(0);

            string save_img_name = str_save_img_path + "/" + str_num_data + ".png";
            cv::imwrite(save_img_name, gray, compression_params);
            
            
        }
        
    }

    outFile.close();

    return 0;
}
