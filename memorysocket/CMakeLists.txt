cmake_minimum_required(VERSION 3.25)
project(memorysocket)

set(CMAKE_CXX_STANDARD 17)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../vendor)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../vendor/libsodium/include)
add_library(memorysocket SHARED memory_socket.cpp
        ServerSocket.cpp
        ServerSocket.h
        ClientSocket.cpp
        ClientSocket.h)
set_target_properties(memorysocket PROPERTIES PUBLIC_HEADER memory_socket.h)
install(TARGETS memorysocket)
