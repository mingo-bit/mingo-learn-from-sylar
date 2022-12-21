cmake_minimum_required(VERSION 3.1)
set(CMAKE_C_COMPILER "/apps/mingo/bin/gcc")
set(CMAKE_CXX_COMPILER "/apps/mingo/bin/g++")
project(mingo_server)

set(CMAKE_VERBOSE_MAKEFILE ON)
add_definitions("-O0 -g -ggdb -Wno-unused-variable")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-function -Wno-builtin-macro-redefined -Wno-deprecated -Wno-deprecated-declarations")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include (cmake/utils.cmake)

include_directories(.)
include_directories(/usr/local/include)
link_directories(/usr/local/lib)

set(LIB_SRC
     mingo/log.cpp
     mingo/util.cpp
     mingo/config.cpp
	 mingo/env.cpp
     mingo/thread.cpp
     mingo/fiber.cpp
     mingo/scheduler.cpp
     mingo/iomanager.cpp
     mingo/timer.cpp
	 mingo/hook.cpp
	 mingo/fd_manager.cpp
	 mingo/address.cpp
	 mingo/socket.cpp
	 mingo/bytearray.cpp
	 mingo/http/http.cpp
	 mingo/http/http11_parser.rl.cpp
	 mingo/http/httpclient_parser.rl.cpp
	 mingo/http/http_parser.cpp
	 mingo/tcp_server.cpp
	 mingo/stream.cpp
	 mingo/streams/socket_stream.cpp
	 mingo/http/http_server.cpp
	 mingo/http/http_session.cpp
	 mingo/http/servlet.cpp
	 mingo/http/http_connection.cpp
	 mingo/uri.cpp
	 mingo/daemon.cpp
     )


add_library(mingo SHARED ${LIB_SRC}) 
force_redefine_file_macro_for_sources(mingo)

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

#add_executable(test_config3 tests/test_config3.cpp)
#add_dependencies(test_config3 mingo)
#target_link_libraries(test_config3 mingo yaml-cpp)

add_executable(test_config_to_log tests/test_config_to_log.cpp)
add_dependencies(test_config_to_log mingo)
force_redefine_file_macro_for_sources(test_config_to_log)
target_link_libraries(test_config_to_log mingo yaml-cpp pthread dl)

add_executable(test_thread tests/test_thread.cpp)
add_dependencies(test_thread mingo)
force_redefine_file_macro_for_sources(test_thread)
target_link_libraries(test_thread mingo yaml-cpp pthread dl)

add_executable(test_util tests/test_util.cpp)
add_dependencies(test_util mingo)
force_redefine_file_macro_for_sources(test_util)
target_link_libraries(test_util mingo yaml-cpp pthread dl)

add_executable(test_fiber tests/test_fiber.cpp)
add_dependencies(test_fiber mingo)
force_redefine_file_macro_for_sources(test_fiber)
target_link_libraries(test_fiber mingo yaml-cpp pthread dl)

add_executable(test_scheduler tests/test_scheduler.cpp)
add_dependencies(test_scheduler mingo)
force_redefine_file_macro_for_sources(test_scheduler)
target_link_libraries(test_scheduler mingo yaml-cpp pthread dl)

add_executable(test_iomanager tests/test_iomanager.cpp)
add_dependencies(test_iomanager mingo)
force_redefine_file_macro_for_sources(test_iomanager)
target_link_libraries(test_iomanager mingo yaml-cpp pthread dl)

add_executable(test_hook tests/test_hook.cpp)
add_dependencies(test_hook mingo)
force_redefine_file_macro_for_sources(test_hook)
target_link_libraries(test_hook mingo yaml-cpp pthread dl)

add_executable(test_address tests/test_address.cpp)
add_dependencies(test_address mingo)
force_redefine_file_macro_for_sources(test_address)
target_link_libraries(test_address mingo yaml-cpp pthread dl)

add_executable(test_socket tests/test_socket.cpp)
add_dependencies(test_socket mingo)
force_redefine_file_macro_for_sources(test_socket)
target_link_libraries(test_socket mingo yaml-cpp pthread dl)

add_executable(test_bytearray tests/test_bytearray.cpp)
add_dependencies(test_bytearray mingo)
force_redefine_file_macro_for_sources(test_bytearray)
target_link_libraries(test_bytearray mingo yaml-cpp pthread dl)

add_executable(test_http tests/test_http.cpp)
add_dependencies(test_http mingo)
force_redefine_file_macro_for_sources(test_http)
target_link_libraries(test_http mingo yaml-cpp pthread dl)

add_executable(test_http_parser tests/test_http_parser.cpp)
add_dependencies(test_http_parser mingo)
force_redefine_file_macro_for_sources(test_http_parser)
target_link_libraries(test_http_parser mingo yaml-cpp pthread dl)

add_executable(test_tcp_server tests/test_tcp_server.cpp)
add_dependencies(test_tcp_server mingo)
force_redefine_file_macro_for_sources(test_tcp_server)
target_link_libraries(test_tcp_server mingo yaml-cpp pthread dl)

add_executable(echo_server examples/echo_server.cpp)
add_dependencies(echo_server mingo)
force_redefine_file_macro_for_sources(echo_server)
target_link_libraries(echo_server mingo yaml-cpp pthread dl)

add_executable(test_http_server tests/test_http_server.cpp)
add_dependencies(test_http_server mingo)
force_redefine_file_macro_for_sources(test_http_server)
target_link_libraries(test_http_server mingo yaml-cpp pthread dl)

add_executable(test_uri tests/test_uri.cpp)
add_dependencies(test_uri mingo)
force_redefine_file_macro_for_sources(test_uri)
target_link_libraries(test_uri mingo yaml-cpp pthread dl)

add_executable(test_http_connection tests/test_http_connection.cpp)
add_dependencies(test_http_connection mingo)
force_redefine_file_macro_for_sources(test_http_connection)
target_link_libraries(test_http_connection mingo yaml-cpp pthread dl)

add_executable(test_daemon tests/test_daemon.cpp)
add_dependencies(test_daemon mingo)
force_redefine_file_macro_for_sources(test_daemon)
target_link_libraries(test_daemon mingo yaml-cpp pthread dl)
	
add_executable(test_env tests/test_env.cpp)
add_dependencies(test_env mingo)
force_redefine_file_macro_for_sources(test_env)
target_link_libraries(test_env mingo yaml-cpp pthread dl)

add_executable(http_server examples/http_server.cpp)
add_dependencies(http_server mingo)
force_redefine_file_macro_for_sources(http_server)
target_link_libraries(http_server mingo yaml-cpp pthread dl)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

