#include "Vec3.h"
#include "Camera.h"
#include "Tup.h"
#include <algorithm>
#include <iostream>
#include <math.h>

quaternion vec3::to_quaternion() {
    return quaternion(axis);
}

vec3 vec3::cross(quaternion const& other) {
    return vec3(glm::cross(other.quat, axis));
}

vec3 vec3::operator*(quaternion const& other) {
    return vec3(axis*other.quat);
}