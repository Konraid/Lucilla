__kernel void CalculateLight(__global float* o_data, __global float* positions,
                             float omega, float gratingWaveLength, float cStepWith, long numCOperations) {

    int gid = get_global_id(0);

    float aX = positions[0];
    float aY = positions[1];
    float bX = positions[2];
    float bY = positions[3];

    int cOffset = 6;
    float cStartX = positions[cOffset];
    float cStartY = positions[cOffset+1];
    float cEndX = positions[cOffset+2];
    float cEndY = positions[cOffset+3];

    float ratio = (float) gid/numCOperations;

    float cX = cStartX + ratio * (cEndX - cStartX);
    float cY = cStartY + ratio * (cEndX - cStartX);

    float s = sqrt(pow((aX-cX), 2) + pow((aY-cY), 2)
            + sqrt(pow((bX-cX), 2) + pow((bY-cY), 2)));

    float time = s / (299792458);
    float phi = omega*time;
    o_data[get_global_id(0)] = phi;
}