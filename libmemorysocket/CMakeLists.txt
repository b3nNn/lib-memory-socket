cmake_minimum_required(VERSION 3.25)
project(memorysocket)

set(CMAKE_CXX_STANDARD 17)
add_library(memorysocket SHARED
        ServerSocket.cpp
        ServerSocket.h
        ClientSocket.cpp
        ClientSocket.h
        ISocket.h
        ServerConfiguration.cpp
        ServerConfiguration.h
        ClientConfiguration.cpp
        ClientConfiguration.h
        Base64Encoding.h
        KeyPairGenerator.h)
target_include_directories(memorysocket PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
