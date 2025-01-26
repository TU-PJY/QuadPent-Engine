#version 460 core

layout(std430, binding = 0) buffer MatrixInput {
    mat4 M1;
    mat4 M2;
    mat4 M3;
    mat4 M4;
    mat4 M5;
};

layout(std430, binding = 1) buffer MatrixOutput {
    mat4 Result;
};

layout(local_size_x = 4, local_size_y = 4, local_size_z = 1) in;

void main() {
    uint Row = gl_GlobalInvocationID.y;
    uint Col = gl_GlobalInvocationID.x;

    if(Row >= 4 || Col >= 4) 
        return;
    
    mat4 Temp12   = M1 * M2;
    mat4 Temp123  = Temp12 * M3;
    mat4 Temp1234 = Temp123 * M4;
    mat4 finalM  = Temp1234 * M5;

    Result[Row][Col] = finalM[Row][Col];
}