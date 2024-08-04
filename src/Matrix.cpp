#include "Matrix.h"
#include "Quaternion.h"

matrix<glm::mat4x4>::matrix(const quaternion& quat) : mat(quat.quat) {}
matrix<glm::mat3x3>::matrix(const quaternion& quat) : mat(quat.quat) {}

matrix<glm::mat4x4>::matrix(quaternion * quat) : mat(quat->quat) {}
matrix<glm::mat3x3>::matrix(quaternion * quat) : mat(quat->quat) {}

// RAW CONSTRUCTORS

matrix<glm::mat2x2>::matrix(float x0, float y0, float x1, float y1) : mat(x0, y0, x1, y1) {}
matrix<glm::mat3x2>::matrix(float x0, float y0, float x1, float y1, float x2, float y2) : mat(x0, y0, x1, y1, x2, y2) {}
matrix<glm::mat4x2>::matrix(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3) : mat(x0, y0, x1, y1, x2, y2, x3, y3) {}

matrix<glm::mat2x3>::matrix(float x0, float y0, float z0, float x1, float y1, float z1) : mat(x0, y0, z0, x1, y1, z1) {}
matrix<glm::mat3x3>::matrix(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2) : mat(x0, y0, z0, x1, y1, z1, x2, y2, z2) {}
matrix<glm::mat4x3>::matrix(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3) : mat(x0, y0, z0, x1, y1, z1, x2, y2, z2, x3, y3, z3) {}

matrix<glm::mat2x4>::matrix(float x0, float y0, float z0, float w0, float x1, float y1, float z1, float w1) : mat(x0, y0, z0, w0, x1, y1, z1, w1) {}
matrix<glm::mat3x4>::matrix(float x0, float y0, float z0, float w0, float x1, float y1, float z1, float w1, float x2, float y2, float z2, float w2) : mat(x0, y0, z0, w0, x1, y1, z1, w1, x2, y2, z2, w2) {}
matrix<glm::mat4x4>::matrix(float x0, float y0, float z0, float w0, float x1, float y1, float z1, float w1, float x2, float y2, float z2, float w2, float x3, float y3, float z3, float w3) : mat(x0, y0, z0, w0, x1, y1, z1, w1, x2, y2, z2, w2, x3, y3, z3, w3) {}

// OPERATORS AND STUFF

vec3 matrix<glm::mat4x4>::get_up() {
    return glm::vec3(mat[0][1], mat[1][1], mat[2][1]);
}

vec3 matrix<glm::mat4x4>::get_right() {
    return glm::vec3(mat[0][0], mat[1][0], mat[2][0]);
}

vec3 matrix<glm::mat4x4>::get_forward() {
    return glm::vec3(mat[0][2], mat[1][2], mat[2][2]);
}

vec3 matrix<glm::mat3x3>::get_up() {
    return glm::vec3(mat[0][1], mat[1][1], mat[2][1]);
}

vec3 matrix<glm::mat3x3>::get_right() {
    return glm::vec3(mat[0][0], mat[1][0], mat[2][0]);
}

vec3 matrix<glm::mat3x3>::get_forward() {
    return glm::vec3(mat[0][2], mat[1][2], mat[2][2]);
}

quaternion matrix<glm::mat3x3>::to_quaternion() {
    return glm::quat(mat);
}

quaternion matrix<glm::mat4x4>::to_quaternion() {
    return glm::quat(mat);
}