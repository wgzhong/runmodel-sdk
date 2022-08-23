 #include "runmodel.h"
 #include<iostream> 
 #include<fstream>

 runmodel::runmodel(std::string jsonFile) {
    std::ifstream jfile(jsonFile);
    if (!jfile.is_open()) {
        return;
    }
    jfile >> m_json_file;
 }

 void runmodel::init(tvm::runtime::ShapeTuple input_shape,
                     tvm::runtime::ShapeTuple output_shape){
     DLDevice m_dev = {kDLCPU, 0};
     if(m_json_file.at("model_device") == "kDLCPU"){
        m_dev = {kDLCPU, 0};
     } else if(m_json_file.at("model_device") == "kDLCUDA"){
        m_dev = {kDLCUDA, 0};
     } else if(m_json_file.at("model_device") == "kDLCUDAHost"){
        m_dev = {kDLCUDAHost, 0};
     } else if(m_json_file.at("model_device") == "kDLOpenCL"){
        m_dev = {kDLOpenCL, 0};
     } else if(m_json_file.at("model_device") == "kDLVulkan"){
        m_dev = {kDLVulkan, 0};
     } else if(m_json_file.at("model_device") == "kDLMetal"){
        m_dev = {kDLMetal, 0};
     } else if(m_json_file.at("model_device") == "kDLVPI"){
        m_dev = {kDLVPI, 0};
     } else if(m_json_file.at("model_device") == "kDLROCM"){
        m_dev = {kDLROCM, 0};
     } else if(m_json_file.at("model_device") == "kDLExtDev"){
        m_dev = {kDLExtDev, 0};
     } else{
        LOG(ERROR)<<"not support this devide:"<<m_json_file.at("model_device");
     }  
     uint8_t dtype = kDLFloat;
     if(m_json_file.at("model_dtype") == "kDLInt"){
        dtype = kDLInt;
     } else if(m_json_file.at("model_dtype") == "kDLUInt"){
        dtype = kDLUInt;
     } else if(m_json_file.at("model_dtype") == "kDLFloat"){
        dtype = kDLFloat;
     } else if(m_json_file.at("model_dtype") == "kDLOpaqueHandle"){
        dtype = kDLOpaqueHandle;
     } else if(m_json_file.at("model_dtype") == "kDLBfloat"){
        dtype = kDLBfloat;
     } else if(m_json_file.at("model_dtype") == "kDLComplex"){
        dtype = kDLComplex;
     } else{
        LOG(ERROR)<<"not support this type:"<<m_json_file.at("model_dtype");
     }   
     uint8_t bits = m_json_file.at("model_bits");
     m_input = tvm::runtime::NDArray::Empty(input_shape, DLDataType{dtype, bits, 1}, m_dev);
     m_output = tvm::runtime::NDArray::Empty(output_shape, DLDataType{dtype, bits, 1}, m_dev);
     m_load_lib = tvm::runtime::Module::LoadFromFile(m_json_file.at("model_so"));
     m_mod = m_load_lib.GetFunction("default")(m_dev);
     m_set_input = m_mod.GetFunction("set_input");
     m_get_output = m_mod.GetFunction("get_output");
     m_run = m_mod.GetFunction("run");
 }

 json runmodel::get_json(){
    return m_json_file;
 }

 void runmodel::set_input(float *data, int size){
    memcpy(m_input->data, data, size);
 }

 void runmodel::run(std::string input_node){
     m_set_input(input_node, m_input);
     m_run();
     m_get_output(0, m_output);
 }

 float* runmodel::get_output() {
     float* result = static_cast<float*>(m_output->data);
 }
 