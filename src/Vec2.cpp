#include "Vec2.h"
#include "Vec3.h"
#include "Vec4.h"
#include "Matrix.h"

vec2::vec2(const vec3& rhs) : axis(rhs.axis.x, rhs.axis.y) {}

vec2::vec2(const vec4& rhs) : axis(rhs.axis.x, rhs.axis.y) {}

vec4 vec2::to_vec4(float z, float w) {
    return vec4(axis, z, w);
}

vec3 vec2::to_vec3(float z) {
    return vec3(axis, z);
}

matrix<glm::mat2x2> vec2::outer_product(const vec2& vec) {
    return glm::outerProduct(axis, vec.axis);
}

matrix<glm::mat3x2> vec2::outer_product(const vec3& vec) {
    return glm::outerProduct(axis, vec.axis);
}

matrix<glm::mat4x2> vec2::outer_product(const vec4& vec) {
    return glm::outerProduct(axis, vec.axis);
}