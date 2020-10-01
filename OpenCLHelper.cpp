#include "OpenCLHelper.h"

cl::Program CreateProgram(const std::string& file) {

    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);

    auto platform = platforms.front();
    std::vector<cl::Device> devices;
    platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);

    auto device = devices.front();

    std::ifstream ifs("HelloWorld.cl");
    std::string content( (std::istreambuf_iterator<char>(ifs) ),
                         (std::istreambuf_iterator<char>()    ) );


    std::ifstream input(file);
    std::stringstream sstr;

    while(input >> sstr.rdbuf());

    std::cout << sstr.str() << std::endl;

    std::cout << "FILE: " << "" << std::endl;

    std::ifstream fileStream(file);
    std::string src(std::istreambuf_iterator<char>(fileStream), (std::istreambuf_iterator<char>()));

    if(src.empty())
    {
        std::cout << "ICH WERDE AUSGEFUEHRT";
    }
    std::cout << src.c_str() << std::endl;

    cl::Program::Sources sources( 1, std::make_pair(src.c_str(), src.length() + 1));

    cl::Context context(device);
    cl::Program program(context, sources);

    program.build("-cl-std=CL1.2");
    return program;
}