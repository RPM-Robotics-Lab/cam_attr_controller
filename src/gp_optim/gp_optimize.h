#ifndef GPOPTIMIZE_H
#define GPOPTIMIZE_H

#include <iostream>
#include <vector>
#include <eigen3/Eigen/Core>
#include <unsupported/Eigen/MatrixFunctions>

using namespace std;
using namespace Eigen;

//MatrixXd gp_cov_k_SE (VectorXd x_i, VectorXd x_j, double l, double s_f) {
//    int dim = x_i.size();
//    cout << dim << endl;
//    double inv_l = 1/(l*l);
//    MatrixXd M = MatrixXd::Identity(dim, dim);
//    M = inv_l * M;

//    VectorXd x_diff = x_i - x_j;
//    MatrixXd cov;
//    cov = -0.5*x_diff.transpose()*M*x_diff;
//    cov = (s_f*s_f)*cov.exp();

//    return cov;

//}


class GPOptimize
{
public:
    GPOptimize();
    void initialize(double ls, double sigma);
    int evaluate(double metric);

    void add_data(double t_val, double y_val);


    MatrixXd train();
    MatrixXd train(vector<VectorXd> x_train, vector<VectorXd> y_train);

    void set_predict(vector<VectorXd> &x_pred);

    void predict();


protected:
    MatrixXd gp_cov_k_SE (VectorXd x_i, VectorXd x_j, double l, double s_f);

private:
    double ls_;
    double sigma_;

    vector<VectorXd> x_train_;
    vector<VectorXd> y_train_;
    vector<VectorXd> x_pred_;
    MatrixXd K_;

    VectorXd y_pred_;
    MatrixXd var_pred_;






};

#endif // GPOPTIMIZE_H
