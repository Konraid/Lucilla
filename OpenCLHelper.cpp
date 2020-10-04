#include "OpenCLHelper.h"

cl::Program CreateProgram(const std::string& file) {

    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);

    auto platform = platforms.front();
    std::vector<cl::Device> devices;
    platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);

    auto device = devices.front();

    std::ifstream fileStream(file);
    std::string src(std::istreambuf_iterator<char>(fileStream), (std::istreambuf_iterator<char>()));

    cl::Program::Sources sources( 1, std::make_pair(src.c_str(), src.length() + 1));

    cl::Context context(device);
    cl::Program program(context, sources);

    if (program.build() != CL_SUCCESS) {
        std::cout << "Error building: " << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device) << std::endl;
        exit(1);
    }

    return program;
}