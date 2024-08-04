#include "Vec4.h"
#include "Camera.h"
#include "Tup.h"
#include <algorithm>
#include <iostream>
#include <math.h>
#include "Vec3.h"
#include "Vec2.h"
#include "Matrix.h"

vec4 vec4::operator*(quaternion const& other) {
    return axis * other.quat;
}

vec4 vec4::operator*(quaternion const& other) const {
    return axis * other.quat;
}

vec3 vec4::to_vec3() {
    return vec3(axis.x, axis.y, axis.z);
}

vec2 vec4::to_vec2() {
    return vec2(axis.x, axis.y);
}

vec4::vec4(const vec3& rhs, float w) : axis(rhs.axis, w) {}

vec4::vec4(const vec2& rhs, float z, float w) : axis(rhs.axis, z, w) {}

matrix<glm::mat2x4> vec4::outer_product(const vec2& vec) {
    return glm::outerProduct(axis, vec.axis);
}

matrix<glm::mat3x4> vec4::outer_product(const vec3& vec) {
    return glm::outerProduct(axis, vec.axis);
}

matrix<glm::mat4x4> vec4::outer_product(const vec4& vec) {
    return glm::outerProduct(axis, vec.axis);
}