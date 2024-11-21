#version 460 core

layout(local_size_x = 4, local_size_y = 4, local_size_z = 1) in;

layout (std430, binding = 0) buffer MatrixInput {
    mat4 Matrix1;
    mat4 Matrix2;
    mat4 Matrix3;
    mat4 Matrix4;
    mat4 Matrix5;
};

layout (std430, binding = 1) buffer MatrixOutput {
    mat4 Result;
};

void main() {
    Result = Matrix1 * Matrix2 * Matrix3 * Matrix4 * Matrix5;
}