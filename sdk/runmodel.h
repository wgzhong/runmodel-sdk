 #ifndef RUNMODEL_H
 #define RUNMODEL_H
 #include <iostream>
 #include <tvm/runtime/module.h>
 #include <tvm/runtime/packed_func.h>
 #include "json.hpp"

 using json = nlohmann::json;
 class runmodel
 {
 public:
     runmodel(std::string jsonFile);
     void init(tvm::runtime::ShapeTuple input_shape, tvm::runtime::ShapeTuple output_shape);
     void init_tvm(DLDevice dev);
     void init_tvm(int device_type = kDLCPU, int device_id = 0);
     void set_input(float *data, int size);
     void run(std::string input_node);
     float* get_output();
     json get_json();

 private:
     json m_json_file;
     tvm::runtime::PackedFunc m_set_input;
     tvm::runtime::PackedFunc m_get_output;
     tvm::runtime::PackedFunc m_run;
     tvm::runtime::NDArray m_input;
     tvm::runtime::NDArray m_output;
 };

 #endif // RUNMODEL_H
