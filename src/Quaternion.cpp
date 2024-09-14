#include "Quaternion.h"
#include "Vec3.h"
#define _USE_MATH_DEFINES
#include <math.h>

quaternion quaternion::from_axis_angle(vec3 axis, float angle) {
    return glm::angleAxis(angle, axis.get_normalized().axis);
}  
 
void quaternion::rotate(vec3 axis, float angle) {
    quat = glm::angleAxis(angle, axis.get_normalized().axis) * quat;
}

vec3 quaternion::to_euler() {
    return glm::eulerAngles(quat);
}

quaternion quaternion::from_euler(const vec3& euler_vec) {
    return quaternion(glm::quat(euler_vec.axis));
}

quaternion quaternion::from_unit(vec3 axis) {
    return glm::rotation(glm::vec3(0.0f, 0.0f, 1.0f), axis.axis);
}

vec3 quaternion::operator*(vec3 const& other)
{
    return vec3(quat*other.axis);
}

vec3 quaternion::cross(vec3 const& other)
{
    return glm::cross(other.axis, quat);
}

vec3 quaternion::get_up() {
    return *this * vec3(0.0f, 1.0f, 0.0f);
}

vec3 quaternion::get_right() {
    return *this * vec3(1.0f, 0.0f, 0.0f);
}

vec3 quaternion::get_forward() {
    return *this * vec3(0.0f, 0.0f, 1.0f);
}