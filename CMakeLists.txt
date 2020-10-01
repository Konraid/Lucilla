cmake_minimum_required(VERSION 3.16)
project(Lucilla)

set(CMAKE_CXX_STANDARD 20)

add_executable(Lucilla main.cpp)

find_package(OpenCL REQUIRED)
include_directories(${OpenCL_INCLUDE_DIRS})
link_directories(${OpenCL_LIBRARY})

target_include_directories (Lucilla PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries (Lucilla ${OpenCL_LIBRARY})