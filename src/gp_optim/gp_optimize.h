#ifndef GPOPTIMIZE_H
#define GPOPTIMIZE_H

#include <iostream>
#include <vector>
#include <eigen3/Eigen/Core>
#include <unsupported/Eigen/MatrixFunctions>

#include "config.h"

using namespace std;
using namespace Eigen;

class GPOptimize
{
public:
    GPOptimize();
    void initialize(double ls, double s_f, double s_n, vector<VectorXd> &x_pred);
    void initialize(double ls, double s_f, double s_n);
    void initialize(Config &cfg);
    void initialize();

    bool evaluate(double x_val, double y_val);
    bool evaluate(VectorXd &x_val, double y_val);

    void add_data(double x_val, double y_val);
    void add_data(VectorXd &x_vec, double y_val);

    MatrixXd train();
    MatrixXd train(vector<VectorXd> x_train, vector<VectorXd> y_train);

    void set_predict(vector<double> &x_pred);
    void set_predict(vector<VectorXd> &x_pred);

    void predict();

    void find_query_point();

    bool is_optimal() { return is_optimal_; }

    double query_exposure() { return query_exposure_; }
    int query_index() { return query_index_; }

    double optimal_expose() { return optimal_exposure_; }
    double optimal_gain() { return optimal_gain_; }
    int optimal_index() { return optimal_index_; }

    VectorXd optimal_attr() { return optimal_attr_; }

protected:
    MatrixXd gp_cov_k_SE (VectorXd x_i, VectorXd x_j, double l, double s_f);
    void check_optimal();
    void set_optimal();

private:
    Config cfg_;
    bool is_optimal_;
    int iter_count_;

    // training input
    vector<VectorXd> x_train_;
    vector<VectorXd> y_train_;

    // inference input
    vector<VectorXd> x_pred_;

    // kernel matrix
    MatrixXd K_;

    // inference output
    VectorXd y_pred_;
    MatrixXd var_pred_;

    // Param for acq by mi (TODO: move to acq class)
    double psi_;
    double alpha_;

    double query_exposure_;
    int query_index_;

    double optimal_exposure_;
    double optimal_gain_;
    int optimal_index_;
    VectorXd optimal_attr_;

    double cost_;

};

#endif // GPOPTIMIZE_H
