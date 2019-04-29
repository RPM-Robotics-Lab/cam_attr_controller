#include <iostream>
#include <sstream>
#include <fstream>
#include <irp_gpgo++/gp_optimize.h>
#include <irp_gpgo++/config.h>

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

int main(int argc, char** argv)
{
    VectorXd x_i(2);
    x_i << 2, 2;
    VectorXd x_j(2);
    x_j << 3, 3;
    double ls = 3000.0;
    double s_f = 35.0;
    double s_n = 10.0;

//    ifstream file_path("/home/yg/svn/docs/papers/conferences/icra2018-jwkim/trunk/matlab/dat/38datar.csv");
    ifstream file_path("../../data/090823a.csv");
    ifstream file_path2("../../data/0909.csv");
    string line;
    vector<double> x_data, x_data2;
    vector<double> y_data, y_data2;

    // Load two sample data
    load_csv_from_file(file_path, x_data, y_data);
    load_csv_from_file(file_path2, x_data2, y_data2);

    cout << "Data load done " << endl
         << "\t Data1: " << x_data.size() << endl
         << "\t Data2: " << x_data2.size() << endl;

    // GPO initialize (set once, globally)
    GPOptimize gpo;
    Config cfg(ls, s_f, s_n, AcqType::MAXVAR, 5);

    // For first frame
    gpo.initialize(cfg);
    gpo.set_predict(x_data); // query exposure range

    double x = x_data[2]; // current exposure (minimum exposure not good for initialize)
    double y = y_data[2]; // current metric
    double best_exposure = x_data[2]; // for safety

    while (!gpo.is_optimal()) {
//    for (int i = 0; i < 20; ++i) {
        cout << "Current query exposure " << x <<", " << y << endl;
        if (gpo.evaluate(x, y)) {
            best_exposure = gpo.optimal_expose();
            break;
        }
        else {
            int next_index = gpo.query_index();
            // double next_exposure = gpo.query_exposure(); // This should be used for real capture
            x = x_data[next_index];
            y = y_data[next_index];
        }
    }
    cout << "DONE!! Best exposure for 37 is " << best_exposure << endl;

    // For second frame
    gpo.initialize(cfg);
    gpo.set_predict(x_data2); // query exposure range

    x = x_data2[2]; // current exposure
    y = y_data2[2]; // current metric
    best_exposure = x_data2[2]; // for safety

    while (!gpo.is_optimal()) {
//    for (int i = 0; i < 20; ++i) {

        cout << "Current query exposure " << x <<", " << y << endl;
        if (gpo.evaluate(x, y)) {
            best_exposure = gpo.optimal_expose();
            break;
        }
        else {
            int next_index = gpo.query_index();
            x = x_data2[next_index];
            y = y_data2[next_index];
        }
    }
    cout << "DONE!! Best exposure for 38 is " << best_exposure << endl;


}
