#include "camera.h"
#include "macro_debug/debug.h"
camera::camera(int video_id, int defatlt_w, int defatlt_h)
{
    #if DEBUG_VIDOE
    cap.open(video_id);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, defatlt_h);
    cap.set(cv::CAP_PROP_FRAME_WIDTH, defatlt_w);
    cap.set(CV_CAP_PROP_FPS, 24);
    if (!cap.isOpened()){
        LOG(INFO)<<"camera open error.........";
    }
    #endif
}

int camera::getimage(cv::Mat &read_image){
    #if DEBUG_VIDOE
    if (!cap.read(read_image)) {
        return -2;
    }
    #endif
    return 0;
}
