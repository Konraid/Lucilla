__kernel void ProcessArray(__global int* i_data, __global int* o_data) {
    o_data[ get_global_id(0) ] = 10;
}