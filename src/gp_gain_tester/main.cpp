#include <iostream>
#include <sstream>
#include <fstream>
#include <gp_optimize/gp_optimize.h>
#include <gp_optimize/config.h>
using namespace Eigen;

void load_csv_from_file (ifstream& file_path, vector<double>& x_data, vector<double>& y_data) {
    string line;

    // Load data
    while(getline(file_path, line))
    {
        int index = 0;
        std::stringstream  lineStream(line);
        std::string        cell;
        while(std::getline(lineStream,cell,','))
        {
            double x, y;
            if(index == 0){
                x = std::strtod(cell.c_str(),0);
                x_data.push_back(x);
            }else if(index == 1){
                y = std::strtod(cell.c_str(),0);
                y_data.push_back(y);
            }
            index ++;
        }
    }
}

void load_csv_from_file (ifstream& file_path, vector<VectorXd>& x_data, vector<double>& y_data) {
    string line;

    // Load data
    while(getline(file_path, line))
    {
        int index = 0;
        std::stringstream  lineStream(line);
        std::string        cell;
        double exposure, gain, metric;
        std::cout << "READ " << std::endl;
        while(std::getline(lineStream,cell,','))
        {
            std::cout << "LINE " << std::endl;

            if (index == 0){
                exposure = std::strtod(cell.c_str(),0);
            }
            else if (index == 1) {
                gain = std::strtod(cell.c_str(),0);
                VectorXd data(2);
                data << exposure, gain;
                x_data.push_back(data);
                std::cout << "LINE " << std::endl;

            }
            else if (index == 2){
                metric = std::strtod(cell.c_str(),0);
                y_data.push_back(metric);
            }
            index ++;
        }
    }
}

int main(int argc, char** argv)
{
    VectorXd x_i(2);
    x_i << 2, 2;
    VectorXd x_j(2);
    x_j << 3, 3;
    double ls = 5.5;
    double s_f = 15.0;
    double s_n = 5.0;
    int num_iter = 20;

//    ifstream file_path("/home/yg/svn/docs/papers/conferences/icra2018-jwkim/trunk/matlab/dat/38datar.csv");
    ifstream file_path("../../data/exp_gain.csv");
    ifstream file_path2("../../data/exp_gain2.csv");
    string line;
    vector<VectorXd> x_data, x_data2;
    vector<double> y_data, y_data2;

    // Load two sample data
    load_csv_from_file(file_path, x_data, y_data);
    load_csv_from_file(file_path2, x_data2, y_data2);

    cout << "Data load done " << endl
         << "\t Data1: " << x_data.size() << endl
         << "\t Data2: " << x_data2.size() << endl;

    // GPO initialize (set once, globally)
    GPOptimize gpo;
    Config cfg(ls, s_f, s_n, AcqType::MAXMI, 0.5, num_iter);

    // For first frame
    gpo.initialize(cfg);
    gpo.set_predict(x_data); // query exposure range

    VectorXd x = x_data[2]; // current exposure (minimum exposure not good for initialize)
    double y = y_data[2]; // current metric
    double best_exposure = x_data[2](0); // for safety
    double best_gain = x_data[2](1);
    double best_metric = y_data[2];
    while (!gpo.is_optimal()) {
//    for (int i = 0; i < 20; ++i) {
        cout << "Current query exposure / gain / metric:\t" << x(0) << " / " << x(1) <<" / " << y << endl;
        if (gpo.evaluate(x, y)) {
            best_exposure = gpo.optimal_expose();
            best_gain = gpo.optimal_gain();
            best_metric = y_data[gpo.optimal_index()];
            break;
        }
        else {
            int next_index = gpo.query_index();
            // double next_exposure = gpo.query_exposure(); // This should be used for real capture
            x = x_data[next_index];
            y = y_data[next_index];
        }
    }
    cout << "DONE!! Best exposure / gain for data 1:\t" << best_exposure << " / " << best_gain << " / " << best_metric << endl << endl;

    // For second frame
    gpo.initialize(cfg);
    gpo.set_predict(x_data2); // query exposure range

    x = x_data2[2]; // current exposure
    y = y_data2[2]; // current metric
    best_exposure = x_data2[2](0); // for safety
    best_gain = x_data2[2](1);
    best_metric = y_data2[2];

    while (!gpo.is_optimal()) {
//    for (int i = 0; i < 20; ++i) {

        cout << "Current query exposure / gain / metric:\t" << x(0) << " / " << x(1) <<" / " << y << endl;
        if (gpo.evaluate(x, y)) {
            best_exposure = gpo.optimal_expose();
            best_gain = gpo.optimal_gain();
            best_metric = y_data[gpo.optimal_index()];
            break;
        }
        else {
            int next_index = gpo.query_index();
            x = x_data2[next_index];
            y = y_data2[next_index];
        }
    }
    cout << "DONE!! Best exposure / gain for data 2:\t" << best_exposure << " / " << best_gain << " / " << best_metric << endl;


}
