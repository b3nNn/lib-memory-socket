cmake_minimum_required(VERSION 3.25)
project(simple_program)
set(CMAKE_CXX_STANDARD 17)

add_executable(simple_program main.cpp)
target_link_libraries(simple_program memorysocket libsodium)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../memorysocket)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../vendor)
