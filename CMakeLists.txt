cmake_minimum_required(VERSION 3.25)
project(lib-memory-socket)

include(cmake/FindSodium.cmake)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

find_package(Sodium REQUIRED)
add_subdirectory(libmemorysocket)
add_subdirectory(example/simple-program)

install(TARGETS memorysocket FILE_SET HEADERS)
