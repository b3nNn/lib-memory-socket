cmake_minimum_required(VERSION 3.25)
project(simple_program)
set(CMAKE_CXX_STANDARD 17)

add_executable(simple_program main.cpp)
include_directories(simple_program ${CMAKE_CURRENT_SOURCE_DIR}/../../libmemorysocket)
include_directories(simple_program ${sodium_INCLUDE_DIR})
target_link_libraries(simple_program memorysocket ${sodium_LIBRARY_RELEASE})

if (WIN32)
    include_directories(simple_program PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../vendor)
endif()