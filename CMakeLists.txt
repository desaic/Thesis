cmake_minimum_required(VERSION 2.6)
project(PhysLang)
find_package(LLVM)

if( NOT LLVM_FOUND )
  message(FATAL_ERROR "LLVM package can't be found. Set CMAKE_PREFIX_PATH variable to LLVM's installation prefix.")
endif()

include_directories( ${LLVM_INCLUDE_DIRS} )
link_directories( ${LLVM_LIBRARY_DIRS} )

llvm_map_components_to_libraries(REQ_LLVM_LIBRARIES jit native)

target_link_libraries(phc ${REQ_LLVM_LIBRARIES})

include_directories("include")
if (CMAKE_BUILD_TYPE MATCHES "Debug")
  message(STATUS "Debug build")
  add_definitions(-DNDEBUG)
  set(CMAKE_CXX_FLAGS_DEBUG ${DEBUG_FLAGS}) 
  add_executable
else(CMAKE_BUILD_TYPE MATCHES "Release")
  message(STATUS "Release build")
  add_definitions(-DDEBUG)
  set(CMAKE_CXX_FLAGS_RELEASE ${RELEASE_FLAGS}) 
endif()
