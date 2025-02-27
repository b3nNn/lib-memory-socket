cmake_minimum_required(VERSION 3.25)
project(memorysocket)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

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

if (WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
    include_directories(memorysocket PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../vendor)
    include_directories(memorysocket PUBLIC $ENV{sodium_DIR}/include)
    target_link_libraries(memorysocket ${sodium_LIBRARY_RELEASE})
endif()

