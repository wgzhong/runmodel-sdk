#include "yolov5s.h"
#include <tvm/runtime/logging.h>

yolov5s::yolov5s()
{
    m_output.clear();
}

void yolov5s::process_input(cv::Mat input_image, float *out_data, int w, int h) {
    assert(!input_image.empty());
    cv::Mat dst, input;
    letterbox(input_image, dst, w, h);
    cv::cvtColor(dst, input, cv::COLOR_BGR2RGB);
    Mat_to_CHW(input, out_data, w, h);
}

void yolov5s::postprocess(float *result, float class_num, float output_shape, 
        float CONF_THRES, float IOU_THRES, float MAX_NUM){
    m_output.clear();
    non_max_suppression(result, m_output, CONF_THRES, IOU_THRES, MAX_NUM, class_num, output_shape);
}

void yolov5s::get_output(std::vector<ground_truth> &output) {
    output = m_output;
}

void yolov5s::Mat_to_CHW(cv::Mat &input_frame, float *data, int w, int h) {
    assert(data && !input_frame.empty());
    unsigned int volChl = w * h;
    for(int c = 0; c < 3; ++c) {
        for (unsigned j = 0; j < volChl; ++j) {
            data[c*volChl + j] = static_cast<float>(float(input_frame.data[j * 3 + c]) / 255.0);
        }
    }
}

int yolov5s::load_input_hex(std::string path, float *data) {
    std::ifstream is(path , std::ifstream::in | std::ios::binary);
	// 2. 计算图片长度
	is.seekg(0, is.end);
	int length = is.tellg();
	is.seekg(0, is.beg);
    LOG(INFO) << "length = "<<length;
    if(length < 1){
        LOG(INFO) <<"not data, please input image or text or bin";
        return -1;
    }
	char * buffer = new char[length];
	is.read(buffer, length);
	is.close();
    memcpy(data, buffer, length);
    delete [] buffer;
    return 0;
}

std::vector<std::string> yolov5s::read_label(std::string path) {
    std::vector<std::string> label;
    std::ifstream is(path);
    std::string  line; 
    while(getline(is, line))
    {   
        if(line.empty()){
            continue;
        }
        if(line[line.size()-1] == '\r'){
            line = line.substr(0, line.size()-1);
        }
        label.push_back(line); 
    }
    return label;
} 

void yolov5s::letterbox(cv::Mat image, cv::Mat &output, int new_w, int new_h) {
    int w = image.cols;
    int h = image.rows;
    float ratio = std::min(float((new_w*1.0)/(w*1.0)), float((new_h*1.0)/(h*1.0)));
    int new_uppad_w = int(std::round(w*ratio));
    int new_uppad_h = int(std::round(h*ratio));
    int dw = (new_w - new_uppad_w) / 2;
    int dh = (new_h - new_uppad_h) / 2;
    cv::Mat resize_image;
    if((w != new_uppad_w) || (h != new_uppad_h)){
        cv::resize(image, resize_image, cv::Size(new_uppad_w, new_uppad_h));
    }
    if(resize_image.empty()){
        resize_image = image;
    }
    int top = std::round(dh - 0.1);
    int bottom = std::round(dh + 0.1);
    int left = std::round(dw - 0.1);
    int right = std::round(dw + 0.1);
    cv::copyMakeBorder(resize_image, output, top, bottom, left, right, cv::BORDER_CONSTANT, cv::Scalar(114,114,114));
}

bbox yolov5s::xywh2xyxy(float x, float y, float w, float h, int shift) {
    bbox output_box;
    output_box.x1 = x - (w / 2.0) + shift;
    output_box.x2 = x + (w / 2.0) + shift;
    output_box.y1 = y - (h / 2.0) + shift;
    output_box.y2 = y + (h / 2.0) + shift;
    return output_box;
}

std::vector<int> yolov5s::argsort(const std::vector<ground_truth>& array) {
	const int array_len(array.size());
	std::vector<int> array_index(array_len, 0);
	for (int i = 0; i < array_len; ++i)
		array_index[i] = i;
	std::sort(array_index.begin(), array_index.end(),
		[&array](int pos1, int pos2) {return (array[pos1].class_prob > array[pos2].class_prob);});
	return array_index;
}

float yolov5s::soft_iou(bbox b1, bbox b2) {
    float maxx1 = std::max(b1.x1, b2.x1);
    float minx2 = std::min(b1.x2, b2.x2);
    float maxy1 = std::max(b1.y1, b2.y1);
    float miny2 = std::min(b1.y2, b2.y2);
    float h = miny2 - maxy1 + 1;
    float w = minx2 - maxx1 + 1;
    if(h < 0 || w < 0){
        return 0;
    }
    float inarea = w*h;
    float area1 = (b1.x2 - b1.x1 + 1)*(b1.y2 - b1.y1 + 1);
    float area2 = (b2.x2 - b2.x1 + 1)*(b2.y2 - b2.y1 + 1);
    float iou = inarea / (area1 + area2 - inarea);
    return iou;
}

void yolov5s::non_max_suppression(float *data, std::vector<ground_truth> &output,  float conf_thres, 
    float iou_thres, int max_nms, int class_num, int output_num) {
    int stride = class_num+4+1;
    std::vector<ground_truth> gt_v;
    for(int n=0; n<output_num*stride; n=n+stride){
        float center_x = data[n];
        float center_y = data[n+1];
        float width = data[n+2];
        float hight = data[n+3];
        float conf = data[n+4];
        // LOG(INFO)<<center_x<<" "<<center_y<<" "<<width<<" "<<hight<<" "<<conf;
        if(conf < conf_thres){
            continue;
        }
        float max_class_conf=0.0;
        int idx=0;
        for (int i = n+5; i < n+class_num+5; i++){
            if(data[i] > max_class_conf){
                max_class_conf = data[i];
                idx = (i-5) % stride;
            }
        }
        max_class_conf = max_class_conf*conf;
        if(max_class_conf < conf_thres){
            continue;
        }
        int shift = 0;
        bbox box = xywh2xyxy(center_x, center_y, width, hight, shift);
        ground_truth gt;
        gt.box = box;
        gt.class_prob = max_class_conf;
        gt.label_idx = idx;
        gt_v.push_back(gt);
    }
    std::vector<int> idx = argsort(gt_v);
    // for(int i=0;i<idx.size();i++){
    //     LOG(INFO)<<gt_v[idx[i]].class_prob<<" "<<gt_v[idx[i]].label_idx<<" "<<gt_v[idx[i]].box.x1<<" "<<gt_v[idx[i]].box.y1<<" "<<gt_v[idx[i]].box.x2;
    // }
    while(idx.size() > 0){
        int k = idx[0];
        if(gt_v[k].class_prob > conf_thres && gt_v[k].class_prob > 0.1){
            output.push_back(gt_v[k]);
        }
        std::vector<int> idx_tmp;
        for(int i = 1; i < idx.size(); i++){
            float iou = soft_iou(gt_v[k].box, gt_v[idx[i]].box);
            if(iou <= iou_thres){
                idx_tmp.push_back(i-1);
            }
        }
        std::vector<int> i_t = idx;
        idx.clear();
        for(int i=0;i<idx_tmp.size();i++){
            idx.push_back(i_t[idx_tmp[i]+1]);
        }
    }
}

