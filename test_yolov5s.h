#include "runmodel.h"
#include "yolov5s.h"
#include "camera.h"
#include <tvm/runtime/logging.h>
#include <opencv2/core.hpp> 
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp> 
#include <opencv2/highgui.hpp>
#include <iostream>
#include <string>

class entrance{
public:
    entrance(int cam_id, int w, int h, int c){
        m_label.clear();
        m_width = w;
        m_hight = h;
        m_depth = c;
        // m_cam = new camera(cam_id, w, h);
        // m_model = new yolov5s();
        // tvm::runtime::ShapeTuple input_shape = {1, m_depth, m_width, m_hight};
        // tvm::runtime::ShapeTuple output_shape = {1, 1008, 54+4+1};
        // m_runmodel = new runmodel(kDLCPU, "./relay_yolov5s.so", input_shape, output_shape, kDLFloat, 32);
        // m_label = m_model->read_label("./classes.txt");
    }
    void run_cam(){
        // m_cam->getimage(m_input_image);
        m_input_image=cv::imread("/home/wgzhong/datasets/poker_new/train/images/poker_452.jpg");//test
        assert(!m_input_image.empty());
    }

    tvm::runtime::NDArray set_input(){
        float *out_data;
        m_model->process_input(m_input_image, out_data, m_width, m_hight);
        //load_input_hex("/home/wgzhong/card-detection/tvm/yolov5_poker/python/cam_image.bin", data);
        // dump_data<float>(m_data, "./input_cpp.bin", m_width * m_hight * m_depth*4);//float
        memcpy(m_input->data, out_data, m_depth * m_width * m_hight*2);
        return m_input;
    }

    void run(){
        m_runmodel->run("images", m_input);
    }

    std::vector<ground_truth> get_output(){
        float *output;
        m_runmodel->get_output(output);
        m_model->postprocess(output, 54, 1008);
        m_output = m_model->get_output();
        return m_output;
    }

    std::vector<std::string> get_label(){
        return m_label;
    }

    void print_output(){
        if(m_output.size()>0){
            for(int i=0;i<1;i++){
                LOG(INFO)<<m_output[i].class_prob<<" label= "<<m_output[i].label_idx << " classes= " << m_label[m_output[i].label_idx];
            }
        }
    }

private:
    tvm::runtime::NDArray m_input;
    std::vector<ground_truth> m_output;
    std::vector<std::string> m_label;
    cv::Mat m_input_image;
    camera *m_cam;
    yolov5s *m_model;
    runmodel *m_runmodel;
    int m_hight;
    int m_width;
    int m_depth;
};