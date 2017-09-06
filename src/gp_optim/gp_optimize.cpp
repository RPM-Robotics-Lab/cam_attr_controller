#include "gp_optimize.h"

GPOptimize::GPOptimize()
{

}

void GPOptimize::initialize(double ls, double sigma)
{
    ls_ = ls;
    sigma_ = sigma;
}


int GPOptimize::evaluate(double metric)
{

}

void GPOptimize::add_data(double t_val, double y_val)
{
    VectorXd t_vec(1);
    t_vec << t_val;

    VectorXd y_vec(1);
    y_vec << y_val;

    x_train_.push_back(t_vec);
    y_train_.push_back(y_vec);
}

MatrixXd GPOptimize::train()
{
    MatrixXd K = train(x_train_, y_train_);

    return K;
}

MatrixXd GPOptimize::train(vector<VectorXd> x_train, vector<VectorXd> y_train)
{
    int num_data = x_train.size();
    MatrixXd K(num_data, num_data);

    for (int i = 0; i < num_data; ++i) {
        for (int j = 0; j < num_data; ++j) {
            VectorXd x_i = x_train[i];
            VectorXd x_j = x_train[j];
            K.block(i,j,1,1) = gp_cov_k_SE(x_i, x_j, ls_, sigma_);
            if (i != j)
                K.block(j,i,1,1) = K.block(i,j,1,1);
        }
    }

    K_ = K;
    return K;

}

void GPOptimize::set_predict(vector<VectorXd>& x_pred)
{
    x_pred_ = x_pred;
}

void GPOptimize::predict()
{
    int n_train = x_train_.size();
    int n_pred = x_pred_.size();
    MatrixXd K_star(n_train, n_pred);

    for (int i = 0; i < n_train; ++i) {
        for (int j = 0; j < n_pred; ++j) {
            K_star.block(i,j,1,1) = gp_cov_k_SE(x_train_[i], x_pred_[j], ls_, sigma_);
        }
    }

    double s_n = 10;
    MatrixXd N = (s_n)*MatrixXd::Identity(n_train, n_train);
    MatrixXd K = K_ + N;
    MatrixXd invK = K.inverse();

    VectorXd y_train(n_train);
    for (int i = 0; i < n_train; ++i) {
        y_train(i) = y_train_[i](0);
    }
    VectorXd mean = K_star.transpose() * invK * y_train;
    MatrixXd K_star2 = gp_cov_k_SE(x_pred_[0], x_pred_[0], ls_, sigma_);
    MatrixXd var = MatrixXd::Constant(n_pred, n_pred, K_star2(0,0)) - K_star.transpose() * invK * K_star;

    y_pred_ = mean;
    var_pred_ = var;

//    cout << "K_star " << K_star << endl;
//    cout << "inv K " << invK << endl;
//    cout << "mean " << endl << mean << endl;
//    cout << "var " << endl << var;
}

MatrixXd GPOptimize::gp_cov_k_SE(VectorXd x_i, VectorXd x_j, double l, double s_f)
{
    int dim = x_i.size();
    double inv_l = 1/(l*l);
    MatrixXd M = MatrixXd::Identity(dim, dim);
    M = inv_l * M;

    VectorXd x_diff = x_i - x_j;
    MatrixXd cov;
    cov = -0.5*x_diff.transpose()*M*x_diff;
    cov = (s_f*s_f)*cov.exp();

    return cov;
}
