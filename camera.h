#ifndef CAMERA_H
#define CAMERA_H
#include <iostream>
#include <opencv2/opencv.hpp>
#include <tvm/runtime/logging.h>

class camera
{
public:
    camera(int video_id, int defatlt_w, int defatlt_h);
    int getimage(cv::Mat &read_image);

private:
    cv::VideoCapture cap;
};

#endif // CAMERA_H