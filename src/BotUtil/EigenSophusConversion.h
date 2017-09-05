#pragma once
#include <sophus/se3.hpp>
#include <sophus/so3.hpp>
#include <Eigen/Geometry>

namespace bot_util
{

void sophus_se3f_to_eigen_isometry3f (Sophus::SE3f& src, Eigen::Isometry3f& dst);

}