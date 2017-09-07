#ifndef GPOPTIMIZE_H
#define GPOPTIMIZE_H

#include <iostream>
#include <vector>
#include <eigen3/Eigen/Core>
#include <unsupported/Eigen/MatrixFunctions>

#include "config.h"

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
    void initialize(double ls, double s_f, double s_n, vector<VectorXd> &x_pred);
    void initialize(double ls, double s_f, double s_n);
    bool evaluate(double x_val, double y_val);

    void add_data(double x_val, double y_val);

    MatrixXd train();
    MatrixXd train(vector<VectorXd> x_train, vector<VectorXd> y_train);

    void set_predict(vector<double> &x_pred);
    void set_predict(vector<VectorXd> &x_pred);

    void predict();

    void find_query_point();

    bool is_optimal() { return is_optimal_; }

    double query_exposure() { return query_exposure_; }
    int query_index() { return query_index_; }

    double optimal_expose() {return optimal_exposure_; }

protected:
    MatrixXd gp_cov_k_SE (VectorXd x_i, VectorXd x_j, double l, double s_f);
    void check_optimal();
    void set_optimal();

private:
    Config cfg_;


    bool is_optimal_;
    double ls_;
    double sigma_;

    vector<VectorXd> x_train_;
    vector<VectorXd> y_train_;
    vector<VectorXd> x_pred_;
    MatrixXd K_;

    VectorXd y_pred_;
    MatrixXd var_pred_;

    double query_exposure_;
    int query_index_;

    double optimal_exposure_;

    double cost_;

};

#endif // GPOPTIMIZE_H
