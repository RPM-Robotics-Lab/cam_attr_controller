#include <unistd.h>
#include <iostream>
#include <lcm/lcm-cpp.hpp>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
using namespace boost::program_options;

#include "Bluefox2Driver/bluefox2.h"
#include "Bluefox2Driver/Bluefox2Config.h"
#include "param-util/param_util.h"
#include "lcmtypes++/bot_core/image_sync_t.hpp"

#include "Parser.h"

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

    int exposureTime_us = 8000;
    int frameRate_Hz = 40;

    bluefox2::Bluefox2 cam_bluefox2(parser.GetSerial());

    bluefox2::Bluefox2Config config = parser.GetConfig();
    cam_bluefox2.Configure(config);
    cam_bluefox2.set_timeout_ms(200); // 200

    cam_bluefox2.SetFPSandExposeTime(frameRate_Hz, exposureTime_us);

    cam_bluefox2.Configure(config);
    cam_bluefox2.set_timeout_ms(200);

    
    lcm::LCM lcm;

    while(1) {
        
        cam_bluefox2.RequestSingle();
        
        bot_core::image_t test_img;
        bot_core::image_sync_t test_img_sync_t;
        cam_bluefox2.GrabImage(test_img);
        test_img_sync_t.utime = test_img.utime;
        
        lcm.publish ("bluefox2-mono", &test_img);
        lcm.publish ("bluefox2-mono.SYNC", &test_img_sync_t);

    }

    return 0;
}
