#include "Vec3.h"
#include "Camera.h"
#include "Tup.h"
#include <algorithm>
#include <iostream>
#include <math.h>
#include "Vec2.h"
#include "Vec4.h"
#include "Matrix.h"

quaternion vec3::to_quaternion() {
    return quaternion(axis);
}

vec3 vec3::cross(quaternion const& other) {
    return glm::cross(other.quat, axis);
}

vec3 vec3::operator*(quaternion const& other) {
    return axis * other.quat;
}

quaternion vec3::to_quaternion() const {
    return quaternion(axis);
}

vec3 vec3::cross(quaternion const& other) const {
    return glm::cross(other.quat, axis);
}

vec3 vec3::operator*(quaternion const& other) const {
    return axis * other.quat;
}

vec4 vec3::to_vec4(float w) {
    return vec4(axis, w);
}

vec2 vec3::to_vec2() {
    return vec2(axis.x, axis.y);
}

vec3::vec3(const vec2& rhs, float z) : axis(rhs.axis, z) {}

vec3::vec3(const vec4& rhs) : axis(rhs.axis.x, rhs.axis.y, rhs.axis.z) {}

matrix<glm::mat2x3> vec3::outer_product(const vec2& vec) {
    return glm::outerProduct(axis, vec.axis);
}

matrix<glm::mat3x3> vec3::outer_product(const vec3& vec) {
    return glm::outerProduct(axis, vec.axis);
}

matrix<glm::mat4x3> vec3::outer_product(const vec4& vec) {
    return glm::outerProduct(axis, vec.axis);
}