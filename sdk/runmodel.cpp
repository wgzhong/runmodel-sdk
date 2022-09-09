 #include "runmodel.h"
 #include <iostream> 
 #include <fstream>
 #include <tvm/runtime/module.h>
 #include <tvm/runtime/registry.h>
 #include <tvm/runtime/packed_func.h>
 #include "utils.h"
 runmodel::runmodel(std::string jsonFile) {
    std::ifstream jfile(jsonFile);
    if (!jfile.is_open()) {
        return;
    }
    jfile >> m_json_file;
 }

 void runmodel::init(tvm::runtime::ShapeTuple input_shape,
                     tvm::runtime::ShapeTuple output_shape){
     DLDevice dev = {kDLCPU, 0};
     if(m_json_file.at("model_device") == "kDLCPU"){
        dev = {kDLCPU, 0};
     } else if(m_json_file.at("model_device") == "kDLCUDA"){
        dev = {kDLCUDA, 0};
     } else if(m_json_file.at("model_device") == "kDLCUDAHost"){
        dev = {kDLCUDAHost, 0};
     } else if(m_json_file.at("model_device") == "kDLOpenCL"){
        dev = {kDLOpenCL, 0};
     } else if(m_json_file.at("model_device") == "kDLVulkan"){
        dev = {kDLVulkan, 0};
     } else if(m_json_file.at("model_device") == "kDLMetal"){
        dev = {kDLMetal, 0};
     } else if(m_json_file.at("model_device") == "kDLVPI"){
        dev = {kDLVPI, 0};
     } else if(m_json_file.at("model_device") == "kDLROCM"){
        dev = {kDLROCM, 0};
     } else if(m_json_file.at("model_device") == "kDLExtDev"){
        dev = {kDLExtDev, 0};
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
     m_input = tvm::runtime::NDArray::Empty(input_shape, DLDataType{dtype, bits, 1}, dev);
     m_output = tvm::runtime::NDArray::Empty(output_shape, DLDataType{dtype, bits, 1}, dev);
     int mode_type = 0;
     mode_type = m_json_file.at("model_type");
     if(mode_type == 1){
      init_tvm(kDLCPU, 0);
     } else{
      init_tvm(dev);
     }
     LOG(INFO) << "init done.................."; 
 }

 void runmodel::init_tvm(DLDevice dev){
   std::string lib_path = m_json_file.at("model_single_so");
   if(!isFileExists_ifstream(lib_path)){
        LOG(ERROR)<<lib_path<<" is not exist!!!";
   }
   tvm::runtime::Module load_lib = tvm::runtime::Module::LoadFromFile(lib_path);
   tvm::runtime::Module mod = load_lib.GetFunction("default")(dev);
   m_set_input = mod.GetFunction("set_input");
   m_get_output = mod.GetFunction("get_output");
   m_run = mod.GetFunction("run");
 }

void runmodel::init_tvm(int device_type, int device_id){
   std::string lib_path = m_json_file.at("model_lib");
   if(!isFileExists_ifstream(lib_path)){
      throw std::runtime_error("could not open json file " + lib_path);
   }
   tvm::runtime::Module load_lib = tvm::runtime::Module::LoadFromFile(lib_path);
   std::string model_json = m_json_file.at("model_json");
   std::ifstream json_inn(model_json);
   if(json_inn.fail()){
      throw std::runtime_error("could not open json file " + model_json);
   }
   std::string params_path = m_json_file.at("model_params");
   std::ifstream params_in(params_path, std::ios::binary);
   if(params_in.fail()){
      throw std::runtime_error("could not open json file " + params_path);
   }
   const std::string json_data((std::istreambuf_iterator<char>(json_inn)),
                           std::istreambuf_iterator<char>());
   json_inn.close();
   const std::string params_data((std::istreambuf_iterator<char>(params_in)),
                                    std::istreambuf_iterator<char>());
   params_in.close();
   TVMByteArray params_arr;
   params_arr.data = params_data.c_str();
   params_arr.size = params_data.length();
   
   tvm::runtime::Module mod = 
      (*tvm::runtime::Registry::Get("tvm.graph_executor.create"))(json_data,
                                                            load_lib, device_type, device_id);
   tvm::runtime::PackedFunc load_params = mod.GetFunction("load_params");
   load_params(params_arr);
   m_set_input = mod.GetFunction("set_input");
   m_get_output = mod.GetFunction("get_output");
   m_run = mod.GetFunction("run");    
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
     return result;
 }
 
