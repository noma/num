# Copyright (c) 2017 Matthias Noack <ma.noack.pr@gmail.com>
#
# See accompanying file LICENSE and README for further information.

cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project(libnoma_num LANGUAGES CXX)

set(NOMA_NUM_THIRDPARTY_DIR ${PROJECT_SOURCE_DIR}/thirdparty)

#set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_EXTENSIONS OFF)

# OpenCL kernel handling
set(NOMA_NUM_OpenCL_KERNEL_DIR ${PROJECT_SOURCE_DIR}/cl) # where the kernel files are, convenience
set(NOMA_NUM_OpenCL_KERNEL_HEADER_DIR ${CMAKE_CURRENT_BINARY_DIR}/cl) # where the generated header file will be put, used by create_opencl_kernel_header
message("NOMA_NUM_OpenCL_KERNEL_DIR: " ${NOMA_NUM_OpenCL_KERNEL_DIR})
file(MAKE_DIRECTORY ${NOMA_NUM_OpenCL_KERNEL_HEADER_DIR})
create_opencl_kernel_header(${NOMA_NUM_OpenCL_KERNEL_DIR}/rk_weighted_add.cl ${NOMA_NUM_OpenCL_KERNEL_HEADER_DIR} NOMA_NUM_KERNEL_HEADER_rk_weighted_add)

# static library 
add_library(noma_num STATIC src/noma/num/types.cpp src/noma/num/butcher_tableau.cpp src/noma/num/stepper_type.cpp src/noma/num/types.cpp src/noma/num/rk_method.cpp src/noma/num/rk_stepper.cpp ${NOMA_NUM_KERNEL_HEADER_rk_weighted_add})

# NOTE: we want to use '#include "noma/num/types.hpp"', not '#include "types.hpp"'
target_include_directories(noma_num PUBLIC include ${NOMA_NUM_OpenCL_KERNEL_HEADER_DIR})

# TODO: do we need OpenCL Libraries here or are we implicitly pulling them in from noma_ocl?
target_link_libraries(noma_num noma_ocl noma_bmt noma_typa ${OpenCL_LIBRARIES})

set_target_properties(noma_num PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

add_subdirectory(src)

