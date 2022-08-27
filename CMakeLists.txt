cmake_minimum_required(VERSION 2.8)
project(mingo_server)

set(CMAKE_VERBOSE_MAKEFILE ON)
add_definitions("-O0 -g -ggdb -Wno-unused-variable")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(.)
include_directories(/usr/local/include)
link_directories(/usr/local/lib)

set(LIB_SRC
     mingo/log.cpp
     mingo/util.cpp
     mingo/config.cpp
     )


add_library(mingo SHARED ${LIB_SRC}) 

#add_executable(test_yaml tests/test_yaml.cpp)
#add_dependencies(test_yaml mingo)
#target_link_libraries(test_yaml mingo yaml-cpp)

#add_executable(test_config tests/test_config.cpp)
#add_dependencies(test_config mingo)
#target_link_libraries(test_config mingo yaml-cpp)

#add_executable(test tests/test.cpp)
#add_dependencies(test mingo)
#target_link_libraries(test mingo yaml-cpp)

#add_executable(test_config2 tests/test_config2.cpp)
#add_dependencies(test_config2 mingo)
#target_link_libraries(test_config2 mingo yaml-cpp)

add_executable(test_config3 tests/test_config3.cpp)
add_dependencies(test_config3 mingo)
target_link_libraries(test_config3 mingo yaml-cpp)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

