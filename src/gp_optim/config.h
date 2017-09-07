#ifndef CONFIG_H
#define CONFIG_H
#include <iostream>

enum AcqType {
    MAXVAR = 0, // Maximum variance
    MAXMI       // Maximum (approx) mutual information
};

class Config
{
public:
    Config () { }
    Config(double ls, double s_f, double s_n, AcqType acq_type, double alpha)
    { ls_ = ls; s_f_ = s_f; s_n_ = s_n;
      acq_type_ = acq_type;
      alpha_ = alpha; }

    void set_cfg(double ls, double s_f, double s_n)
    { ls_ = ls; s_f_ = s_f; s_n_ = s_n;
      acq_type_ = AcqType::MAXVAR; }

    void set_cfg(double ls, double s_f, double s_n, AcqType acq_type, double alpha)
    { ls_ = ls; s_f_ = s_f; s_n_ = s_n;
      acq_type_ = acq_type;
      alpha_ = alpha; }

    double ls() { return ls_; }
    double s_f() { return s_f_; }
    double s_n() { return s_n_; }

    void ls(double ls) { ls_ = ls; }
    void s_f(double s_f) { s_f_ = s_f; }
    void s_n(double s_n) { s_n_ = s_n; }

    double alpha() { return alpha_; }
    void alpha(double alpha) { alpha_ = alpha; }

    void acq_type(AcqType acq_type) { acq_type_ = acq_type; }
    AcqType acq_type() { return acq_type_; }

private:
    // Hyperparameters
    double ls_;
    double s_f_;
    double s_n_;

    // Parameters for GPMI
    double alpha_;

    // Acquisition Function Type
    AcqType acq_type_;

};

#endif // CONFIG_H
