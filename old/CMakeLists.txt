cmake_minimum_required(VERSION 2.6)
set(CXX_SOURCES
  src/tokens.cpp
  src/parser.cpp
  src/LexWrapper.cpp
  src/ParserWrapper.cpp
  src/AstType.cpp
  src/AstNode.cpp
  src/primitive/NBool.cpp
  src/primitive/NDouble.cpp
  src/primitive/NFloat.cpp
  src/primitive/NInteger.cpp
  src/NBinaryOp.cpp
  src/NBlock.cpp
  src/NExpression.cpp
  src/NIdentifier.cpp
  src/NIfStatement.cpp
  src/NFor.cpp
  src/NFunctionCall.cpp
  src/NFunctionDeclaration.cpp
  src/NVariableDeclaration.cpp
  src/NReturn.cpp
  src/NStatement.cpp
  src/NStructDeclaration.cpp
  src/SymbolTable.cpp
  src/codegen.cpp
  src/main.cpp
)

set(DEBUG_FLAGS "-Wall -Wextra ")
set(RELEASE_FLAGS "-O2 -Wall -Wextra")

message(STATUS "${CXX_SOURCES}")
project(PhysLang)
find_package(LLVM)
if( NOT LLVM_FOUND )
  message(FATAL_ERROR "LLVM package can't be found. Set CMAKE_PREFIX_PATH variable to LLVM's installation prefix.")
endif()
include(AddLLVM)
add_definitions( -D__STDC_LIMIT_MACROS -D__STDC_CONSTANT_MACROS )
include_directories( ${LLVM_INCLUDE_DIRS} )
message(STATUS "LLVM inc dirs")
message(STATUS "${LLVM_INCLUDE_DIRS}")
link_directories( ${LLVM_LIBRARY_DIRS} )
llvm_map_components_to_libraries(REQ_LLVM_LIBRARIES jit native)

include_directories("include")
if (CMAKE_BUILD_TYPE MATCHES "Debug")
  add_definitions(-DDEBUG)
#  set(CMAKE_CXX_FLAGS_DEBUG ${DEBUG_FLAGS}) 
  set(CMAKE_CXX_FLAGS ${DEBUG_FLAGS})
  message(STATUS "Debug build")
  message(STATUS ${DEBUG_FLAGS})  
else(CMAKE_BUILD_TYPE MATCHES "Release")
  add_definitions(-DNDEBUG)
#  set(CMAKE_CXX_FLAGS_RELEASE ${RELEASE_FLAGS}) 
  set(CMAKE_CXX_FLAGS ${RELEASE_FLAGS})
  message(STATUS "Release build")
  message(STATUS ${CMAKE_CXX_FLAGS_RELEASE})
endif()

add_executable(phc 
  ${CXX_SOURCES})
target_link_libraries(phc LLVMAsmParser
			LLVMBitReader
			LLVMLinker)
target_link_libraries(phc ${REQ_LLVM_LIBRARIES})
