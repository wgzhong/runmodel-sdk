/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

/*!
 * \brief This is an all in one TVM runtime file.
 *
 *   You only have to use this file to compile libtvm_runtime to
 *   include in your project.
 *
 *  - Copy this file into your project which depends on tvm runtime.
 *  - Compile with -std=c++14
 *  - Add the following include path
 *     - /path/to/tvm/include/
 *     - /path/to/tvm/3rdparty/dmlc-core/include/
 *     - /path/to/tvm/3rdparty/dlpack/include/
 *   - Add -lpthread -ldl to the linked library.
 *   - You are good to go.
 *   - See the Makefile in the same folder for example.
 *
 *  The include files here are presented with relative path
 *  You need to remember to change it to point to the right file.
 *
 */
#define TVM_USE_LIBBACKTRACE 0
#include "/home/wgzhong/tvm/src/runtime/c_runtime_api.cc"
#include "/home/wgzhong/tvm/src/runtime/container.cc"
#include "/home/wgzhong/tvm/src/runtime/cpu_device_api.cc"
#include "/home/wgzhong/tvm/src/runtime/file_utils.cc"
#include "/home/wgzhong/tvm/src/runtime/library_module.cc"
#include "/home/wgzhong/tvm/src/runtime/logging.cc"
#include "/home/wgzhong/tvm/src/runtime/module.cc"
#include "/home/wgzhong/tvm/src/runtime/ndarray.cc"
#include "/home/wgzhong/tvm/src/runtime/object.cc"
#include "/home/wgzhong/tvm/src/runtime/registry.cc"
#include "/home/wgzhong/tvm/src/runtime/thread_pool.cc"
#include "/home/wgzhong/tvm/src/runtime/threading_backend.cc"
#include "/home/wgzhong/tvm/src/runtime/workspace_pool.cc"

// NOTE: all the files after this are optional modules
// that you can include remove, depending on how much feature you use.

// Likely we only need to enable one of the following
// If you use Module::Load, use dso_module
// For system packed library, use system_lib_module
#include "/home/wgzhong/tvm/src/runtime/dso_library.cc"
#include "/home/wgzhong/tvm/src/runtime/system_library.cc"

// Graph executor
#include "/home/wgzhong/tvm/src/runtime/graph_executor/graph_executor.cc"
#include "/home/wgzhong/tvm/src/runtime/graph_executor/graph_executor_factory.cc"

// Uncomment the following lines to enable RPC
// #include "/home/wgzhong/tvm/src/runtime/rpc/rpc_session.cc"
// #include "/home/wgzhong/tvm/src/runtime/rpc/rpc_event_impl.cc"
// #include "/home/wgzhong/tvm/src/runtime/rpc/rpc_server_env.cc"

// These macros enables the device API when uncommented.
#define TVM_CUDA_RUNTIME 1
#define TVM_METAL_RUNTIME 1
#define TVM_OPENCL_RUNTIME 1

// Uncomment the following lines to enable Metal
// #include "/home/wgzhong/tvm/src/runtime/metal/metal_device_api.mm"
// #include "/home/wgzhong/tvm/src/runtime/metal/metal_module.mm"

// Uncomment the following lines to enable CUDA
// #include "/home/wgzhong/tvm/src/runtime/cuda/cuda_device_api.cc"
// #include "/home/wgzhong/tvm/src/runtime/cuda/cuda_module.cc"

// Uncomment the following lines to enable OpenCL
// #include "/home/wgzhong/tvm/src/runtime/opencl/opencl_device_api.cc"
// #include "/home/wgzhong/tvm/src/runtime/opencl/opencl_module.cc"
