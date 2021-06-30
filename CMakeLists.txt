cmake_minimum_required(VERSION 3.16)
project(threadlab C)

set(CMAKE_C_STANDARD 99)
add_executable(threadlab main.c threadlab.c threadlab.h)
target_link_libraries(threadlab pthread)
