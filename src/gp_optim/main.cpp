#include <iostream>
#include "gp_optimize.h"

int main(int argc, char** argv)
{
    VectorXd x_i(2);
    x_i << 2, 2;
    VectorXd x_j(2);
    x_j << 3, 3;
    double l = 100;
    double s = 100;

    vector<VectorXd> x_pred;
    double init = 30;
    for (int i = 0; i < 56; ++i) {
        VectorXd x(1);
        x <<init + i*10;

        x_pred.push_back(x);
    }

    GPOptimize gpo;
    gpo.initialize(l, s);

    gpo.add_data(30, 15921);
    gpo.add_data(80, 22866);

    gpo.set_predict(x_pred);

    MatrixXd K = gpo.train();

    gpo.predict();

}
