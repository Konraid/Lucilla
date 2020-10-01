#include <iostream>
#include <fstream>
#include <stdio.h>
#include <CL/cl.hpp>
#include "OpenCLHelper.h"

void printDevicesInfo(std::vector<cl::Device> devices) {
    std::cout << "================================================" << std::endl;
    for (int i = 0; i<devices.size(); i++) {
        auto device = devices[i];
        auto vendor = device.getInfo<CL_DEVICE_VENDOR>();
        auto version = device.getInfo<CL_DEVICE_VERSION>();
        std::cout << "Vendor: " << vendor << ", Version: " << version << std::endl;
    }
    std::cout << "================================================" << std::endl;
}

void memTest() {
    bool DEBUG = true;

    cl::Program program = CreateProgram("../HelloWorld.cl");

    cl::Context context = program.getInfo<CL_PROGRAM_CONTEXT>();
    auto devices = context.getInfo<CL_CONTEXT_DEVICES>();
    auto& device = devices.front();

    if(DEBUG) {
        printDevicesInfo(devices);
    }

    int buf[16];
    cl::Buffer memBuf(context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, sizeof(buf));

    cl::Kernel kernel(program, "HelloWorld");
    kernel.setArg(0, memBuf);

    cl::CommandQueue queue(context, device);
    queue.enqueueTask(kernel);
    queue.enqueueReadBuffer(memBuf, 1, 0, sizeof(buf), buf);

    for (int i = 0; i<16; i++) {
        std::cout << buf[i] << std::endl;
    }

    std::cin.get();
}

int main()
{
    bool DEBUG = true;

    cl::Program program = CreateProgram("../ProcessArray.cl");

    cl::Context context = program.getInfo<CL_PROGRAM_CONTEXT>();
    auto devices = context.getInfo<CL_CONTEXT_DEVICES>();
    auto& device = devices.front();

    if(DEBUG) {
        printDevicesInfo(devices);
    }

    std::vector<int> vec(10);
    std::fill(vec.begin(), vec.end(), 6);

    cl::Buffer inBuf(context, CL_MEM_READ_ONLY | CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR, sizeof(int)
        * vec.size(), vec.data());

    cl::Buffer outBuf(context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, sizeof(int)
                                                                                                 * vec.size());
    cl::Kernel kernel(program, "ProcessArray");
    kernel.setArg(0, inBuf);
    kernel.setArg(1, outBuf);

    cl::CommandQueue queue(context, device);
    //queue.enqueueFillBuffer(inBuf, cl::patt)
    queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(vec.size()));
    queue.enqueueReadBuffer(outBuf, CL_TRUE, 0, sizeof(int) * vec.size(), vec.data());

    cl::finish();


    for (int i = 0; i<16; i++) {
        std::cout << vec[i] << std::endl;
    }

    std::cin.get();

}