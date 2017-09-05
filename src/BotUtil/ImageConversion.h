#pragma once
#include <opencv/cv.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "lcmtypes++/bot_core/image_t.hpp"

namespace bot_util
{

void botimage_to_cvMat (bot_core::image_t *bot_img, cv::Mat& img);

}
