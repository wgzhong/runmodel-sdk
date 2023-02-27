#ifndef UTILS_H
#define UTILS_H
#include <iostream>
#include <fstream>
#include <string>
#include <tvm/runtime/logging.h>
#include "json.hpp"

typedef struct config_def{
    std::string m_mode_name;
    std::string m_model_device;
    std::string m_model_single_so;
    std::string m_model_json;
    std::string m_model_lib;
    std::string m_model_params;
    std::string m_model_true_label;
    std::string m_model_classes_label;
    std::string m_model_datasets;
    std::string m_pic_or_video;
    std::string m_dump;
    std::string m_model_dtype;
    std::string m_model_input_node;
    int m_model_camera_id;
    int m_save_pic;
    int m_model_type;
    int m_model_bits;
    int m_model_input_width;
    int m_model_input_height;
    int m_model_input_depth;
    int m_model_output_classes_num;
    int m_model_output_shape;
} config_def_t;

bool isFileExists_ifstream(std::string& name);
bool get_json_content(std::string jsonFile, config_def_t &json_config);
#endif
