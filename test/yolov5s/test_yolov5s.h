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
#include <vector>
#include "sdk/utils.h"
 
class entrance{
public:
    entrance(config_def_t json_config){
        m_json_config = json_config;
        m_runmodel = new runmodel(json_config);
        tvm::runtime::ShapeTuple input_shape = {1, m_json_config.m_model_input_depth, m_json_config.m_model_input_width, m_json_config.m_model_input_height};
        tvm::runtime::ShapeTuple output_shape = {1, m_json_config.m_model_output_shape, m_json_config.m_model_output_classes_num+4+1};
        m_runmodel->init(input_shape, output_shape);
        m_label.clear();
        if(m_json_config.m_pic_or_video == "video"){
            m_cam = new camera(json_config.m_model_camera_id, m_json_config.m_model_input_width, m_json_config.m_model_input_height);
        }    
        m_model = new yolov5s();
        if(!isFileExists_ifstream(m_json_config.m_model_classes_label)){
            throw std::runtime_error("could not open json file " + m_json_config.m_model_classes_label);
        }
        m_label = m_model->read_label(m_json_config.m_model_classes_label);        
    }

    int get_filenames(std::vector<cv::String> &file_names){
        cv::glob(m_json_config.m_model_datasets + "/*.jpg", m_file_names, true);
        if (m_file_names.size() == 0) {
            cv::glob(m_json_config.m_model_datasets + "/*.png", m_file_names, true);
        }
        file_names = m_file_names;
        return m_file_names.size();
    }
    cv::Mat run_cam(std::string file_path=""){
        if(m_json_config.m_pic_or_video == "pic"){
            m_input_image=cv::imread(file_path);//test
        } else if(m_json_config.m_pic_or_video == "video"){
            m_cam->getimage(m_input_image);
        } else{
            LOG(ERROR)<<"not support other format!\n";
        }
        assert(!m_input_image.empty());
        return m_input_image;
    }

    void set_input(){
        float *out_data = new float[m_json_config.m_model_input_width * m_json_config.m_model_input_height * m_json_config.m_model_input_depth];
        m_model->process_input(m_input_image, out_data, m_json_config.m_model_input_width, m_json_config.m_model_input_height);
        // load_input_hex("/home/wgzhong/card-detection/tvm/yolov5_poker/python/cam_image.bin", out_data);
        if(m_json_config.m_dump == "input"){
            dump_data<float>(out_data, "./input_cpp.bin", m_json_config.m_model_input_width * m_json_config.m_model_input_height * m_json_config.m_model_input_depth*4);//float
        }
        m_runmodel->set_input(out_data, m_json_config.m_model_input_width * m_json_config.m_model_input_height * m_json_config.m_model_input_depth * 2);
    }

    void run(){
        m_runmodel->run();
    }

    std::vector<ground_truth> get_output(){
        float *output = m_runmodel->get_output();
        m_model->postprocess(output, m_json_config.m_model_output_classes_num, m_json_config.m_model_output_shape);
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
    std::vector<cv::String> m_file_names;
    config_def_t m_json_config;
};
