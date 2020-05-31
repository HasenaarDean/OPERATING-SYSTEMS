cmake_minimum_required(VERSION 3.14)
project(os_ex3)

set(CMAKE_CXX_STANDARD 14)

add_executable(os_ex3 main.cpp)
add_executable(SampleClient SampleClient/SampleClient.cpp MapReduceClient.h MapReduceFramework.h MapReduceFramework.cpp)

add_executable(atomicdemo atomicdemo/atomicdemo.cpp)
target_link_libraries(atomicdemo pthread)

add_executable(barrierdemo barrierdemo/barrierdemo.cpp barrierdemo/Barrier.h barrierdemo/Barrier.cpp)
target_link_libraries(barrierdemo pthread)
