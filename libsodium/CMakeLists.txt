find_library(lib sodium ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/libsodium/.libs)
add_library(libsodium SHARED IMPORTED GLOBAL) # GLOBAL -> if outside src tree
set_property(TARGET libsodium PROPERTY IMPORTED_LOCATION ${lib})
set_property(TARGET libsodium APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/libsodium/include)