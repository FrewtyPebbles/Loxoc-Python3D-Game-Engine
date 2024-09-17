#include "Quaternion.h"
#include "Vec3.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "Matrix.h"

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

quaternion quaternion::from_unit(vec3 axis, const vec3 & up) {
    vec3 forward = axis.get_normalized();  // Ensure it's a unit vector
    vec3 right = up.cross(forward).get_normalized();
    vec3 actualUp = forward.cross(right);
    
    glm::mat3 rotationMatrix(right.axis, actualUp.axis, forward.axis);  // Create a 3x3 rotation matrix
    return glm::quat_cast(rotationMatrix);  // Convert to quaternion
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