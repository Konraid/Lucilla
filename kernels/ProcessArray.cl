__kernel void ProcessArray(__global int* i_data, __global int* o_data) {
    o_data[get_global_id(0)] = i_data[get_global_id(0)];
}

__kernel void ProcessArray2(__global int* i_data, __global int* o_data) {

    o_data[get_global_id(0)] = sqrt((float)get_global_id(0));
}

typedef struct point {
    float X;
    float y;
};

__kernel void ComputeRoot(__global int* i_data, __global int* o_data, __global float* A_X, __global float* A_Y,
                          __global float* B_X, __global float* B_Y, __global float* C_X, __global float* C_Y) {

    //float s = sqrt( pow((A_X-C_X), 2) + pow((A_Y-C_Y), 2) ) + sqrt( pow((B_X-C_X), 2) + pow((B_Y-C_Y), 2) );

    o_data[get_global_id(0)] = 1;
}