cmake_minimum_required(VERSION 3.25)
project(lib-memory-socket)

add_subdirectory(memorysocket)
add_subdirectory(example/simple-program)
add_subdirectory(libsodium libsodium) # usage for out of src ref
