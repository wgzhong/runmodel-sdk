#include "runmodel.h"

runmodel::runmodel(DLDeviceType device, std::string lib_path, 
                    tvm::runtime::ShapeTuple input_shape, 
                    tvm::runtime::ShapeTuple output_shape, 
                    uint8_t dtype, uint8_t bits) {   
    m_dev = {device, 0};
    m_input = tvm::runtime::NDArray::Empty(input_shape, DLDataType{dtype, bits, 1}, m_dev);
    m_output = tvm::runtime::NDArray::Empty(output_shape, DLDataType{dtype, bits, 1}, m_dev);
    m_load_lib = tvm::runtime::Module::LoadFromFile(lib_path);
    init();
}

void runmodel::init(){
    m_mod = m_load_lib.GetFunction("default")(m_dev);
    m_set_input = m_mod.GetFunction("set_input");
    m_get_output = m_mod.GetFunction("get_output");
    m_run = m_mod.GetFunction("run");
}

void runmodel::run(std::string input_node, tvm::runtime::NDArray data){
    m_set_input(input_node, data);
    m_run();
    m_get_output(0, m_output);
}

void runmodel::get_output(float* result) {
    result = static_cast<float*>(m_output->data);
}