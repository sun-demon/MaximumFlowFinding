cmake_minimum_required(VERSION 3.23)
project(MaximumFlowFinding)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} "-static")

include_directories(include)
add_executable(MaximumFlowFinding main.cpp include/dinic.h)
