#include "utils.h"
using json = nlohmann::json;

bool isFileExists_ifstream(std::string& name) {
    std::ifstream f(name.c_str());
    return f.good();
}

bool get_json_content(std::string jsonFile, config_def_t &json_config) {
    json json_file;
    std::ifstream jfile(jsonFile);
    if (!jfile.is_open()) {
        LOG(ERROR)<<"json not found\n";
        return false; 
    }
    jfile >> json_file;
    json_config.m_mode_name             = json_file.at("model_name");
    json_config.m_model_device          = json_file.at("model_device");
    json_config.m_model_single_so       = json_file.at("model_single_so");
    json_config.m_model_json            = json_file.at("model_json");
    json_config.m_model_lib             = json_file.at("model_lib");
    json_config.m_model_params          = json_file.at("model_params");
    json_config.m_model_true_label      = json_file.at("model_true_label");
    json_config.m_model_classes_label   = json_file.at("model_classes_label");
    json_config.m_model_datasets        = json_file.at("model_datasets");
    json_config.m_pic_or_video          = json_file.at("pic_or_video");
    json_config.m_dump                  = json_file.at("dump");
    json_config.m_model_dtype           = json_file.at("model_dtype");
    json_config.m_model_input_node      = json_file.at("model_input_node");
    
    json_config.m_save_pic              = json_file.at("save_pic");
    json_config.m_model_camera_id       = json_file.at("model_camera_id");
    json_config.m_model_type            = json_file.at("model_type");
    json_config.m_model_bits            = json_file.at("model_bits");
    json_config.m_model_input_width     = json_file.at("model_input_width");
    json_config.m_model_input_height    = json_file.at("model_input_height");
    json_config.m_model_input_depth     = json_file.at("model_input_depth");
    json_config.m_model_output_classes_num  = json_file.at("model_output_classes_num");
    json_config.m_model_output_shape    = json_file.at("model_output_shape");
    
    
}
