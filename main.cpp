#define __CL_ENABLE_EXCEPTIONS                                      // For debug information in kernel files

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <CL/cl.hpp>
#include <chrono>
#include <cmath>
#include "OpenCLHelper.h"
#include "Constants.h"

using std::cout;
using std::endl;


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

void printInfo() {
    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);

    std::cout << "Found " << platforms.size() << " platforms." << std::endl;
    for (int i = 0; i < platforms.size(); ++i) {
        cl::Platform platform = platforms[i];
        auto profile =  platform.getInfo<CL_PLATFORM_PROFILE>();
        auto version =  platform.getInfo<CL_PLATFORM_VERSION>();
        auto name =  platform.getInfo<CL_PLATFORM_NAME>();
        auto vendor =  platform.getInfo<CL_PLATFORM_VENDOR>();
        auto extensions =  platform.getInfo<CL_PLATFORM_EXTENSIONS>();

        std::cout << "========================================================" << std::endl;
        std::cout << "Platform_ID: " << i << std::endl;
        std::cout << "Profile: " << profile << std::endl;
        std::cout << "Version: " << version << std::endl;
        std::cout << "Name: " << name << std::endl;
        std::cout << "Vendor: " << vendor << std::endl;
        std::cout << "Extensions: " << extensions << std::endl << std::endl;

        std::vector<cl::Device> devices;
        platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);

        std::cout << "Found " << devices.size() << " devices." << std::endl;

        for (int j = 0; j < devices.size(); ++j) {

            cl::Device device = devices[j];

            auto type = device.getInfo<CL_DEVICE_TYPE>();
            auto vendor = device.getInfo<CL_DEVICE_VENDOR>();
            auto vendor_id = device.getInfo<CL_DEVICE_VENDOR_ID>();
            auto name = device.getInfo<CL_DEVICE_NAME>();
            auto driver_version = device.getInfo<CL_DEVICE_VERSION>();
            auto max_compute_units = device.getInfo<CL_DEVICE_MAX_COMPUTE_UNITS>();
            auto max_work_item_units = device.getInfo<CL_DEVICE_MAX_WORK_ITEM_SIZES>();
            auto max_work_group_units = device.getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>();
            auto max_work_clock_freq = device.getInfo<CL_DEVICE_MAX_CLOCK_FREQUENCY>();
            auto max_work_mem_alloc_size = device.getInfo<CL_DEVICE_MAX_MEM_ALLOC_SIZE>();
            auto fp_config = device.getInfo<CL_DEVICE_DOUBLE_FP_CONFIG>();
            auto glob_mem_cache_type = device.getInfo<CL_DEVICE_GLOBAL_MEM_CACHE_TYPE>();
            auto built_in_kernels = device.getInfo<CL_DEVICE_BUILT_IN_KERNELS>();

            std::cout << "---------------------------------------------------------" << std::endl;
            std::cout << "Device_ID: " << j << std::endl;
            std::cout << "Type: " << type << std::endl;
            std::cout << "Vendor: " << vendor << std::endl;
            std::cout << "Vendor_ID: " << vendor_id << std::endl;
            std::cout << "Name: " << name << std::endl;
            std::cout << "Driver_Version: " << driver_version << std::endl;
            std::cout << "CL_DEVICE_MAX_COMPUTE_UNITS: " << max_compute_units << std::endl;
            //std::cout << "CL_DEVICE_MAX_WORK_ITEM_SIZES: " << max_work_item_units << std::endl;
            std::cout << "CL_DEVICE_MAX_WORK_GROUP_SIZE: " << max_work_group_units << std::endl;
            std::cout << "Max Work Clock Freq: " << max_work_clock_freq << std::endl;
            std::cout << "CL_DEVICE_MAX_MEM_ALLOC_SIZE: " << max_work_mem_alloc_size << std::endl;
            std::cout << "CL_DEVICE_DOUBLE_FP_CONFIG: " << fp_config << std::endl;
            std::cout << "CL_DEVICE_GLOBAL_MEM_CACHE_TYPE: " << glob_mem_cache_type << std::endl;
            std::cout << "CL_DEVICE_BUILT_IN_KERNELS: " << built_in_kernels << std::endl;

        }

    }
    
}

void memTest() {
    bool DEBUG = true;

    cl::Program program = CreateProgram("kernels/HelloWorld.cl");

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

int lightCalc(float lightWaveLength, float gratingWaveLength, float positions[10], float cStepWith, float bStepWith, bool DEBUG = false) {
    // print Information
    std::cout << "Starting lightsimulation version " << constants::VERSION << std::endl;

    cl::Program program = CreateProgram("kernels/CalculateLight.cl");

    cl::Context context = program.getInfo<CL_PROGRAM_CONTEXT>();
    auto devices = context.getInfo<CL_CONTEXT_DEVICES>();
    auto device = devices.front();


    float lightSpeed = 299792458;
    float omega = 2 * M_PI * lightSpeed / lightWaveLength;

    int cOffset = 6;
    float cStartX = positions[cOffset];
    float cStartY = positions[cOffset+1];
    float cEndX = positions[cOffset+2];
    float cEndY = positions[cOffset+3];

    float cLength = sqrt(pow(cStartX-cEndX, 2)+pow(cStartY-cEndY, 2));
    long numCOperations = cLength / cStepWith;                                                     // TODO check for no overflow
    std::vector<float> vec(numCOperations);

    std::cout << "Calculations for each B: " << numCOperations << ", " << (float) numCOperations/INT32_MAX * 100 << "%." << std::endl;

    cl::Buffer inBuf(context, CL_MEM_READ_ONLY | CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR, sizeof(float) * 10, positions);
    cl::Buffer outBuf(context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, sizeof(float) * vec.size());
    cl::Kernel kernel(program, "CalculateLight");

    kernel.setArg(0, outBuf);
    kernel.setArg(1, inBuf);
    kernel.setArg(2, sizeof(omega), &omega);
    kernel.setArg(3, sizeof(gratingWaveLength), &gratingWaveLength);
    kernel.setArg(4, sizeof(cStepWith), &cStepWith);
    kernel.setArg(5, sizeof(numCOperations), &numCOperations);

    cl::CommandQueue queue(context, device);
    queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(vec.size()));
    queue.enqueueReadBuffer(outBuf, CL_TRUE, 0, sizeof(float) * vec.size(), vec.data());

    cl::finish();

    /*for (int i = 0; i < 20; ++i) {
        std::cout << vec[i] << std::endl;
    }*/

    // Keeping old stuff for reference
    return 1;

    while (0) {
        cl::Program program = CreateProgram("../ProcessArray.cl");

        cl::Context context = program.getInfo<CL_PROGRAM_CONTEXT>();
        auto devices = context.getInfo<CL_CONTEXT_DEVICES>();
        auto device = devices.front();

        std::vector<int> vec(std::pow(10, 7));
        //std::fill(vec.begin(), vec.end(), 6);

        cl::Buffer inBuf(context, CL_MEM_READ_ONLY | CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR, sizeof(int)
                                                                                                   * vec.size(), vec.data());

        cl::Buffer outBuf(context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, sizeof(int)
                                                                              * vec.size());
        cl::Kernel kernel(program, "ProcessArray");
        kernel.setArg(0, inBuf);
        kernel.setArg(1, outBuf);

        cl::CommandQueue queue(context, device);
        queue.enqueueFillBuffer(inBuf, 3, 0, sizeof(int) * vec.size());
        queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(vec.size()));
        queue.enqueueReadBuffer(outBuf, CL_TRUE, 0, sizeof(int) * vec.size(), vec.data());

        cl::finish();

        for (int i = 0; i < 10; ++i) {
            std::cout << vec[i] << std::endl;
        }
    }

}

int main()
{
    bool DEBUG = false;
    float lightWaveLength = 420 * pow(10, -9);                              // Set default light wavelength to 420 nm
    float gratingWaveLength = 1 * pow(10, -6);                              // Set default grating waveLength to 1 µm
    float cStepWith = 1 * pow(10, -9);                                     // set default c steps to 1 nm
    float bStepWith = 1 * pow(10, -3);                                      // set default b steps to 1 mm

    float positions[10];
    positions[0] = 0.f;                                                             // light source / A x coordinate
    positions[1] = 1.f;                                                             // light source / A y coordinate
    positions[2] = 0.2f;                                                            // B start x coordinate
    positions[3] = 1.f;                                                             // B start y coordinate
    positions[4] = -0.2f;                                                           // B end x coordinate
    positions[5] = 1.f;                                                             // B end y coordinate
    positions[6] = 0.5f;                                                            // C start x coordinate
    positions[7] = 1.f;                                                             // C start y coordinate
    positions[8] = -positions[6];                                                           // C end x coordinate
    positions[9] = 1.f;                                                             // C end y coordinate

    if(DEBUG) {
        printInfo();
    }

    auto t1 = std::chrono::high_resolution_clock::now();

    lightCalc(lightWaveLength, gratingWaveLength, positions, cStepWith, bStepWith, DEBUG);

    auto t2 = std::chrono::high_resolution_clock::now();
    std::cout << "Runtime: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << "ms." << std::endl;
}