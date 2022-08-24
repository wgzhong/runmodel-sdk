#include "sdk/runmodel.h"
#include "yolov5s.h"
#include "camera/camera.h"
#include <tvm/runtime/logging.h>
#include <opencv2/core.hpp> 
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp> 
#include <opencv2/highgui.hpp>
#include <iostream>
#include <string>
#include "tools/tool.h"
#include "macro_debug/debug.h"
#include <vector>
class entrance{
public:
    entrance(std::string json_path){
        m_runmodel = new runmodel(json_path);
        m_json_file = m_runmodel->get_json();
        m_depth = m_json_file.at("model_input_depth");
        m_width = m_json_file.at("model_input_width");
        m_height = m_json_file.at("model_inputt_height");
        m_classes_num = m_json_file.at("model_output_classes_num");
        m_output_shape = m_json_file.at("model_output_shape");
        tvm::runtime::ShapeTuple input_shape = {1, m_depth, m_width, m_height};
        tvm::runtime::ShapeTuple output_shape = {1, m_output_shape, m_classes_num+4+1};
        m_runmodel->init(input_shape, output_shape);
        m_label.clear();
        m_cam = new camera(m_json_file.at("model_camera_id"), m_width, m_height);
        m_model = new yolov5s();
        m_label = m_model->read_label(m_json_file.at("model_classes_label"));        
    }

    int get_filenames(std::vector<cv::String> &file_names){
        std::string data_path = m_json_file.at("model_datasets");
        cv::glob(data_path + "/*.jpg", m_file_names, true);
        file_names = m_file_names;
        return m_file_names.size();
    }
    cv::Mat run_cam(std::string file_path=""){
        #if DEBUG_PIC
        m_input_image=cv::imread(file_path);//test
        #elif DEBUG_VIDOE
        m_cam->getimage(m_input_image);
        #endif
        assert(!m_input_image.empty());
        return m_input_image;
    }

    void set_input(){
        float *out_data = new float[m_width * m_height * m_depth];
        m_model->process_input(m_input_image, out_data, m_width, m_height);
        // load_input_hex("/home/wgzhong/card-detection/tvm/yolov5_poker/python/cam_image.bin", out_data);
        #if DEBUG_DUMP_INPUT
        dump_data<float>(out_data, "./input_cpp.bin", m_width * m_height * m_depth*4);//float
        #endif
        m_runmodel->set_input(out_data, m_depth * m_width * m_height * 2);
    }

    void run(){
        m_runmodel->run(m_json_file.at("model_input_node"));
    }

    std::vector<ground_truth> get_output(){
        float *output = m_runmodel->get_output();
        m_model->postprocess(output, m_classes_num, m_output_shape);
        m_output.clear();
        m_model->get_output(m_output);
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
    std::vector<ground_truth> m_output;
    std::vector<std::string> m_label;
    cv::Mat m_input_image;
    camera *m_cam;
    yolov5s *m_model;
    runmodel *m_runmodel;
    json m_json_file;
    std::vector<cv::String> m_file_names;
    int m_height;
    int m_width;
    int m_depth;
    int m_classes_num;
    int m_output_shape;
};
