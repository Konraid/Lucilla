cmake_minimum_required(VERSION 3.16)
project(Lucilla)

set(CMAKE_CXX_STANDARD 20)

add_executable(Lucilla main.cpp OpenCLHelper.cpp OpenCLHelper.h ProcessArray.cpp ProcessArray.h Constants.h)

find_package(OpenCL REQUIRED)
include_directories(${OpenCL_INCLUDE_DIRS})
link_directories(${OpenCL_LIBRARY})

target_include_directories (Lucilla PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries (Lucilla ${OpenCL_LIBRARY})

file(GLOB KERNEL_FILES
        "kernels/*.cl"
        )
file(COPY ${KERNEL_FILES} DESTINATION ${PROJECT_BINARY_DIR}/kernels/)

configure_file(kernels/CalculateLight.cl ${PROJECT_BINARY_DIR}/kernels/CalculateLight.cl COPYONLY)
#configure_file(kernels/HelloWorld.cl ${RUNTIME_OUTPUT_DIRECTORY}/kernels/HelloWorld.cl COPYONLY)