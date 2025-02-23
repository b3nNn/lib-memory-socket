cmake_minimum_required(VERSION 3.25)
project(memorysocket)

set(CMAKE_CXX_STANDARD 17)
add_library(memorysocket SHARED memory_socket.cpp)
set_target_properties(memorysocket PROPERTIES PUBLIC_HEADER memory_socket.h)
install(TARGETS memorysocket)