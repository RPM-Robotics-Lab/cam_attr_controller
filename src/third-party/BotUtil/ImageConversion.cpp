#include "ImageConversion.h"

namespace bot_util
{

void botimage_to_cvMat (bot_core::image_t *bot_img, cv::Mat& img)
{
    if(bot_img->pixelformat != bot_core::image_t::PIXEL_FORMAT_RGB)
        img = cv::Mat(bot_img->height, bot_img->width, CV_8UC1, &bot_img->data[0]);
    else {
        img = cv::Mat(bot_img->height, bot_img->width, CV_8UC3, &bot_img->data[0]);
        cv::cvtColor(img,img, cv::COLOR_RGB2BGR);
    }

    //cv::namedWindow("test",cv::WINDOW_NORMAL);
    //cv::imshow("test",img);

    //cv::waitKey(1);
}

}
