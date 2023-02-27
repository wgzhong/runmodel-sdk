 #ifndef RUNMODEL_H
 #define RUNMODEL_H
 #include <iostream>
 #include <tvm/runtime/module.h>
 #include <tvm/runtime/packed_func.h>
#include "utils.h"

 class runmodel
 {
 public:
     runmodel(config_def_t json_config);
     void init(tvm::runtime::ShapeTuple input_shape, tvm::runtime::ShapeTuple output_shape);
     void init_tvm(DLDevice dev);
     void init_tvm(int device_type = kDLCPU, int device_id = 0);
     void set_input(float *data, int size);
     void run();
     float* get_output();

 private:
     config_def_t m_json_config;
     tvm::runtime::PackedFunc m_set_input;
     tvm::runtime::PackedFunc m_get_output;
     tvm::runtime::PackedFunc m_run;
     tvm::runtime::NDArray m_input;
     tvm::runtime::NDArray m_output;
 };

 #endif // RUNMODEL_H
