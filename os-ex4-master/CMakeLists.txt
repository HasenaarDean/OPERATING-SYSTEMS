cmake_minimum_required(VERSION 3.12)
project(os_ex4)

set(CMAKE_CXX_STANDARD 11)
add_definitions(-DDEBUG)

add_executable(simple_test SimpleTest.cpp PhysicalMemory.cpp VirtualMemory.cpp util.cpp util.h)

add_executable(write_twice WriteTwice.cpp PhysicalMemory.cpp VirtualMemory.cpp util.cpp util.h)

add_executable(tester tester.cpp PhysicalMemory.cpp VirtualMemory.cpp util.cpp util.h)

add_executable(binaryRepTest BinaryRepTest.cpp PhysicalMemory.cpp PhysicalMemory.h util.cpp util.h)
