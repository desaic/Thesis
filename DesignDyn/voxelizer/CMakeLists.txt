cmake_minimum_required (VERSION 2.6)

project (vox)

set(CMAKE_MODULE_PATH $ENV{CMAKE_MODULE_PATH})

set(VOX_INCLUDE_DIR ${vox_SOURCE_DIR}/include CACHE INTERNAL "" FORCE)
#SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++11")

file(GLOB VOX_SOURCE_CODE "${vox_SOURCE_DIR}/*.cpp"
                          "${vox_SOURCE_DIR}/src/*.cpp" 
                          "${vox_SOURCE_DIR}/vecmath/src/*.cpp")

include_directories(${VOX_INCLUDE_DIR})
include_directories(${vox_SOURCE_DIR}/include/vecmath)

add_executable(vox ${VOX_SOURCE_CODE})