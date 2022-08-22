#include "camera.h"

camera::camera(int video_id = 0, int defatlt_w = 480, int defatlt_h = 480)
{
    // cap.open(video_id);
    // cap.set(cv::CAP_PROP_FRAME_HEIGHT, defatlt_h);
    // cap.set(cv::CAP_PROP_FRAME_WIDTH, defatlt_w);
    // cap.set(CV_CAP_PROP_FPS, 24);
    // if (!cap.isOpened()){
    //     LOG(INFO)<<"camera open error.........";
    // }
}

int camera::getimage(cv::Mat &read_image){
    if (!cap.read(read_image)) {
        return -2;
    }
    return 0;
}
