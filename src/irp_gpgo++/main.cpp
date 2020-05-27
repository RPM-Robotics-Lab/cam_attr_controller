#include <iostream>
#include <sstream>
#include <fstream>
#include "gp_optimize.h"

int main(int argc, char** argv)
{
    VectorXd x_i(2);
    x_i << 2, 2;
    VectorXd x_j(2);
    x_j << 3, 3;
    double l = 100;
    double s = 100;

    std::ifstream file_path("/home/yg/svn/docs/papers/conferences/icra2018-jwkim/trunk/matlab/dat/38datar.csv");
    string line;
    vector<double> x_data;
    vector<double> y_data;

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

//    for (auto x : x_data) {
//        cout << "x = " << x << endl;
//    }
//    for (auto y : y_data) {
//        cout << "y = " << y << endl;
//    }


//    cout << "XDATA" << x_data.size() << endl;
//    cout << "YDATA" << y_data.size() << endl;

    double best_exposure;
    GPOptimize gpo;
    gpo.initialize(l, s);
    gpo.set_predict(x_data);
//    gpo.add_data(x_data[0], y_data[0]);
    double x = x_data[0];
    double y = y_data[0];
//    while (!gpo.is_optimal()) {
    for (int i = 0; i < 20; ++i) {

        if (gpo.evaluate(x, y)) {
            best_exposure = gpo.optimal_expose();
            break;
        }
        else {
            int next_index = gpo.query_index();
            x = x_data[next_index];
            y = y_data[next_index];
        }
    }


//    cout << "DONE!! Best exposure is " << best_exposure << endl;


//    vector<VectorXd> x_pred;
//    double init = 30;
//    for (int i = 0; i < 56; ++i) {
//        VectorXd x(1);
//        x <<init + i*10;

//        x_pred.push_back(x);
//    }

//    GPOptimize gpo;
//    gpo.initialize(l, s, x_pred);

//    gpo.add_data(30, 15921);
//    gpo.add_data(80, 22866);

////    gpo.set_predict(x_pred);

//    MatrixXd K = gpo.train();

//    gpo.predict();

//    gpo.find_query_point();

}
