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

int main()
{
    bool DEBUG = true;

    cl::Program program = CreateProgram("../HelloWorld.cl");

    cl::Context context = program.getInfo<CL_PROGRAM_CONTEXT>();
    auto devices = context.getInfo<CL_CONTEXT_DEVICES>();
    auto& device = devices.front();

    if(DEBUG) {
        printDevicesInfo(devices);
    }

    int buf[16];
    std::cout << sizeof(buf) << std::endl;
    cl::Buffer memBuf(context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, sizeof(buf));

    cl::Kernel kernel(program, "HelloWorld");
    kernel.setArg(0, memBuf);

    cl::CommandQueue queue(context, device);
    queue.enqueueTask(kernel);
    // queue.enqueueReadBuffer(memBuf, 1, 0, sizeof(buf), buf);
    queue.enqueueReadBuffer(memBuf, 2, 0, sizeof(buf), buf);

    char test[16];

    test[0] = 'A';
    test[1] = 'B';
    test[2] = 'C';
    test[3] = 'D';
    test[4] = 'E';
    test[5] = 'F';
    test[6] = 'G';
    test[7] = 'H';
    test[8] = 'I';
    test[9] = 'j';
    test[10] = 'K';

    //std::cout << std::endl << std::endl << test << std::endl << std::endl;


    std::cout << (buf[1]) << "POPO";
    std::cin.get();

}