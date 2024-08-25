#include "Matrix.h"
#include "Quaternion.h"
  
template<>
matrix<glm::mat4x4>::matrix(const quaternion& quat) : mat(glm::toMat4(quat.quat)) {}
template<>
matrix<glm::mat3x3>::matrix(const quaternion& quat) : mat(glm::toMat3(quat.quat)) {}

template<>  
matrix<glm::mat4x4>::matrix(quaternion * quat) : mat(glm::toMat4(quat->quat)) {}
template<>
matrix<glm::mat3x3>::matrix(quaternion * quat) : mat(glm::toMat3(quat->quat)) {}
     
// RAW CONSTRUCTORS
template<>    
matrix<glm::mat2x2>::matrix(float x0, float y0, float x1, float y1) : mat(x0, y0, x1, y1) {}
template<>
matrix<glm::mat3x2>::matrix(float x0, float y0, float x1, float y1, float x2, float y2) : mat(x0, y0, x1, y1, x2, y2) {}
template<>
matrix<glm::mat4x2>::matrix(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3) : mat(x0, y0, x1, y1, x2, y2, x3, y3) {}

template<>
matrix<glm::mat2x3>::matrix(float x0, float y0, float z0, float x1, float y1, float z1) : mat(x0, y0, z0, x1, y1, z1) {}
template<>
matrix<glm::mat3x3>::matrix(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2) : mat(x0, y0, z0, x1, y1, z1, x2, y2, z2) {}
template<>
matrix<glm::mat4x3>::matrix(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3) : mat(x0, y0, z0, x1, y1, z1, x2, y2, z2, x3, y3, z3) {}

template<>
matrix<glm::mat2x4>::matrix(float x0, float y0, float z0, float w0, float x1, float y1, float z1, float w1) : mat(x0, y0, z0, w0, x1, y1, z1, w1) {}
template<>
matrix<glm::mat3x4>::matrix(float x0, float y0, float z0, float w0, float x1, float y1, float z1, float w1, float x2, float y2, float z2, float w2) : mat(x0, y0, z0, w0, x1, y1, z1, w1, x2, y2, z2, w2) {}
template<>
matrix<glm::mat4x4>::matrix(float x0, float y0, float z0, float w0, float x1, float y1, float z1, float w1, float x2, float y2, float z2, float w2, float x3, float y3, float z3, float w3) : mat(x0, y0, z0, w0, x1, y1, z1, w1, x2, y2, z2, w2, x3, y3, z3, w3) {}
     
           
// OPERATORS AND STUFF
    
template<>
vec3 matrix<glm::mat4x4>::get_up() {
    return glm::vec3(mat[0][1], mat[1][1], mat[2][1]);
}

template<>
vec3 matrix<glm::mat4x4>::get_right() { 
    return glm::vec3(mat[0][0], mat[1][0], mat[2][0]);
}
 
template<>
vec3 matrix<glm::mat4x4>::get_forward() {
    return glm::vec3(mat[0][2], mat[1][2], mat[2][2]);
}

template<>
vec3 matrix<glm::mat3x3>::get_up() {
    return glm::vec3(mat[0][1], mat[1][1], mat[2][1]);
} 
 
template<>
vec3 matrix<glm::mat3x3>::get_right() {
    return glm::vec3(mat[0][0], mat[1][0], mat[2][0]);
}

template<>
vec3 matrix<glm::mat3x3>::get_forward() {
    return glm::vec3(mat[0][2], mat[1][2], mat[2][2]);
}

template<>
quaternion matrix<glm::mat3x3>::to_quaternion() {
    return glm::quat(mat);
}
 
template<>
quaternion matrix<glm::mat4x4>::to_quaternion() {
    return glm::quat(mat);
}