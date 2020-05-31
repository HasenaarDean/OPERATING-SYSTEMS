# cmake_minimum_required(VERSION <specify CMake version here>)
project(os_ex1)

set(CMAKE_CXX_STANDARD 11)

add_executable(os_ex1 main.cpp osm.h osm.cpp)