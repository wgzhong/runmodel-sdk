#ifndef CAMERA_H
#define CAMERA_H
#include <iostream>
#include <opencv2/opencv.hpp>
#include <tvm/runtime/logging.h>

class camera
{
public:
    camera(int video_id = 0, int defatlt_w = 480, int defatlt_h = 480);
    int getimage(cv::Mat &read_image);

private:
    cv::VideoCapture cap;
};

#endif // CAMERA_H