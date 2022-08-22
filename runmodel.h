#ifndef RUNMODEL_H
#define RUNMODEL_H
#include <iostream>
#include <tvm/runtime/module.h>
#include <tvm/runtime/packed_func.h>

class runmodel
{
public:
    runmodel(DLDeviceType device, std::string lib_path, 
                    tvm::runtime::ShapeTuple input_shape, 
                    tvm::runtime::ShapeTuple output_shape, 
                    uint8_t dtype = kDLFloat, uint8_t bits = 32);
    void init();
    void run(std::string input_node, tvm::runtime::NDArray data);
    void get_output(float* result);
private:
    DLDevice m_dev;
    tvm::runtime::Module m_load_lib;
    tvm::runtime::Module m_mod;
    tvm::runtime::PackedFunc m_set_input;
    tvm::runtime::PackedFunc m_get_output;
    tvm::runtime::PackedFunc m_run;
    tvm::runtime::NDArray m_input;
    tvm::runtime::NDArray m_output;
};

#endif // RUNMODEL_H
