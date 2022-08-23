#ifndef YOLOV5S_H
#define YOLOV5S_H
#include <iostream>
#include <vector>
#include <opencv2/core.hpp> 
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp> 
#include <opencv2/highgui.hpp>

struct bbox{
    float x1;
    float y1;
    float x2;
    float y2;
    bbox(){
        x1=0.0;
        x2=0.0;
        y1=0.0;
        y2=0.0;
    }
};

struct ground_truth{
    bbox box;
    float class_prob;
    int label_idx;
    ground_truth(){
        class_prob=0.0;
        label_idx=0;
    }
};

class yolov5s
{
public:
    yolov5s();
    void process_input(cv::Mat input_image, float *out_data, int w, int h);
    void postprocess(float *result, float class_num, float output_shape, 
        float CONF_THRES = 0.025, float IOU_THRES = 0.6, float MAX_NUM = 3000);
    void get_output(std::vector<ground_truth> &output);
    void Mat_to_CHW(cv::Mat &input_frame, float *data, int w, int h);
    int load_input_hex(std::string path, float *data);
    std::vector<std::string> read_label(std::string path);
    void letterbox(cv::Mat image, cv::Mat &output, int new_w, int new_h);
    bbox xywh2xyxy(float x, float y, float w, float h, int shift);
    std::vector<int> argsort(const std::vector<ground_truth>& array);
    float soft_iou(bbox b1, bbox b2);
    void non_max_suppression(float *data, std::vector<ground_truth> &output,  float conf_thres, 
        float iou_thres, int max_nms, int class_num, int output_num);

private:
    std::vector<ground_truth> m_output;
};

#endif // YOLOV5S_H