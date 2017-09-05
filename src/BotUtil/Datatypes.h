#pragma once

#include <vector>
#include <memory>
#include <Eigen/Core>
#include <opencv2/opencv.hpp>

using namespace std;

namespace bot_util
{
    union PointXYZI {
        float data[3];
        struct {
            float x;
            float y;
            float z;
            float intensity;
        };
    };

    typedef PointXYZI PointType;
} // namespace lidar_client