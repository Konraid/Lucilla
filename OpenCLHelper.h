#ifndef LUCILLA_OPENCLHELPER_H
#define LUCILLA_OPENCLHELPER_H
#define __CL_ENABLE_EXCEPTIONS

#include "OpenCLHelper.h"
#include <string>
#include <fstream>
#include <CL/cl.hpp>
#include <iostream>
#include <sstream>

cl::Program CreateProgram(const std::string& file);

#endif //LUCILLA_OPENCLHELPER_H
