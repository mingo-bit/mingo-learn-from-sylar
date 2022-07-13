cmake_minimum_required(VERSION 2.8)
project(mingo_server)

set(CMAKE_VERBOSE_MAKEFILE ON)
add_definitions("-O0 -g -ggdb -Wno-unused-variable")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(LIB_SRC
     mingo/log.cpp
     )

add_library(mingo SHARED ${LIB_SRC}) 
add_executable(test tests/test.cpp)
add_dependencies(test mingo)
target_link_libraries(test mingo)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

