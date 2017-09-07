#include <iostream>
#include <sstream>
#include <fstream>
#include <gp_optimize/gp_optimize.h>

int main(int argc, char** argv)
{
    VectorXd x_i(2);
    x_i << 2, 2;
    VectorXd x_j(2);
    x_j << 3, 3;
    double ls = 100;
    double s_f = 100;
    double s_n = 11;

    ifstream file_path("/home/yg/svn/docs/papers/conferences/icra2018-jwkim/trunk/matlab/dat/38datar.csv");
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

    GPOptimize gpo;
    gpo.initialize(ls, s_f, s_n);
    gpo.set_predict(x_data);
    double x = x_data[0];
    double y = y_data[0];
    double best_exposure = x_data[0];

    while (!gpo.is_optimal()) {
//    for (int i = 0; i < 20; ++i) {

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


    cout << "DONE!! Best exposure is " << best_exposure << endl;
}
