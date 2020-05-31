cmake_minimum_required(VERSION 3.12)
project(os_ex2)

set(CMAKE_CXX_STANDARD 14)

add_executable(os_ex2 main.cpp demo uthreads.cpp Scheduler.cpp Scheduler.h ThreadManager.cpp ThreadManager.h)
add_definitions(-DTEST_SCHEDULER)
add_executable(test_schedule Scheduler.cpp Scheduler.h test_scheduler.cpp)
add_executable(demo_itimer demo/demo_itimer.c)
add_executable(demo_jmp demo/demo_jmp.c)
add_executable(demo_singInt_handler demo/demo_singInt_handler.c)
