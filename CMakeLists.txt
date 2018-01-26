cmake_minimum_required(VERSION 3.10) 
project(VALHALLA LANGUAGES CUDA CXX)

INCLUDE (FindPackageHandleStandardArgs)
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

option(BUILD_ArrayFire "Build the ArrayFire benchmark files" ON)
option(BUILD_Thrust_device "Build the Thrust benchmark files" ON)
option(BUILD_Thrust_host "Build the Thrust benchmark files" ON)
option(BUILD_BoostCompute "Build the Boost Compute benchmark files" OFF)
option(BUILD_HSABolt "Build the HSA Bolt files" OFF)
option(BUILD_TBB "Build the TBB files" ON)


SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CUDA_STANDARD 11)
# Compiler Options
SET(CMAKE_CUDA_FLAGS "-gencode arch=compute_61,code=sm_61
                      -gencode arch=compute_52,code=sm_52                      
                      ${CMAKE_CUDA_FLAGS}")

INCLUDE_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}/include")


if (${BUILD_ArrayFire})
    add_subdirectory(src/arrayfire)
endif()

if (${BUILD_Thrust_device})
    add_subdirectory(src/thrust_device)
endif()

if (${BUILD_Thrust_host})
    add_subdirectory(src/thrust_host)
endif()

if (${BUILD_BoostCompute})
    add_subdirectory(src/boost.compute)
endif()

if (${BUILD_HSABolt})
    add_subdirectory(src/hsa.bolt)
endif()

if (${BUILD_TBB})
    add_subdirectory(src/tbb)
endif()
