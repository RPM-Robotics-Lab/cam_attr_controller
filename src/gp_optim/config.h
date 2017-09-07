#ifndef CONFIG_H
#define CONFIG_H
#include <iostream>

class Config
{
public:
    Config() {}
    void set_cfg(double ls, double s_f, double s_n)
    {ls_ = ls; s_f_ = s_f; s_n_ = s_n;}

    double ls() { return ls_; }
    double s_f() { return s_f_; }
    double s_n() { return s_n_; }

    void ls(double ls) { ls_ = ls; }
    void s_f(double s_f) { s_f_ = s_f; }
    void s_n(double s_n) { s_n_ = s_n; }

private:

    double ls_; // Length scale
    double s_f_;
    double s_n_;

};

#endif // CONFIG_H
