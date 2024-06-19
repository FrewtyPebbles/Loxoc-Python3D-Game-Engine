#pragma once
#include <iostream>
#include <math.h>
#include "Tup.h"
#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_SSE42 // or GLM_FORCE_SSE42 if your processor supports it
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/norm.hpp>
#include "Quaternion.h"

// cpp class names are camelcase, python class names are title case
typedef glm::vec3 glmvec3;

class camera;
class quaternion;

class vec3 {
public:
    vec3()=default;
    vec3(float x, float y, float z) : axis(glm::vec3(x,y,z)) {}
    vec3(const glm::vec3& axis) : axis(axis) {}
    vec3(const vec3& rhs) : axis(rhs.axis) {}
    glm::vec3 axis;

    friend inline std::ostream& operator<<(std::ostream& os, const vec3& self){
        os << "vec3<" << self.axis.x << ", " << self.axis.y << ", " << self.axis.z << '>';
        return os;
    }

    inline float get_x() {return axis.x;}
    inline float get_y() {return axis.y;}
    inline float get_z() {return axis.z;}
    inline void set_x(float other) {axis.x = other;}
    inline void set_y(float other) {axis.y = other;}
    inline void set_z(float other) {axis.z = other;}

    inline vec3 get_up() {
        float pitch = axis.x;
        float yaw = axis.y;
        float roll = axis.z;
        return vec3(
            sin(pitch) * sin(yaw),
            cos(pitch),
            sin(pitch) * cos(yaw)
        );
    }

    inline vec3 get_right() {
        float pitch = axis.x;
        float yaw = axis.y;
        float roll = axis.z;
        return vec3(
            cos(yaw),
            0,
            -sin(yaw)
        );
    }

    inline vec3 get_forward() {
        float pitch = axis.x;
        float yaw = axis.y;
        float roll = axis.z;
        return vec3(
            cos(pitch) * sin(yaw),
            -sin(pitch),
            cos(pitch) * cos(yaw)
        );
    }


    quaternion to_quaternion();

    // Operators

    inline vec3 operator-() {
        return vec3(-axis.x, -axis.y, -axis.z);
    }

    inline vec3 operator+(vec3 const& other)
    {
        
        return vec3(axis.x+other.axis.x, axis.y+other.axis.y, axis.z+other.axis.z);
    }

    inline vec3 operator+(float const& other)
    {
        return vec3(axis.x+other, axis.y+other, axis.z+other);
    }

    inline vec3 operator-(vec3 const& other)
    {
        return vec3(axis.x-other.axis.x, axis.y-other.axis.y, axis.z-other.axis.z);
    }

    inline vec3 operator-(float const& other)
    {
        return vec3(axis.x-other, axis.y-other, axis.z-other);
    }
    
    inline vec3 operator*(vec3 const& other)
    {
        return vec3(axis.x*other.axis.x, axis.y*other.axis.y, axis.z*other.axis.z);
    }

    inline vec3 operator*(float const& other)
    {
        return vec3(axis.x*other, axis.y*other, axis.z*other);
    }
    
    inline vec3 operator/(vec3 const& other)
    {
        return vec3(axis.x/other.axis.x, axis.y/other.axis.y, axis.z/other.axis.z);
    }

    inline vec3 operator/(float const& other)
    {
        return vec3(axis.x/other, axis.y/other, axis.z/other);
    }

    inline float dot(vec3 const& other)
    {
        return glm::dot(this->axis, other.axis);
    }

    inline vec3 cross(vec3 const& other)
    {
        return vec3(glm::cross(this->axis, other.axis));
    }

    inline float get_magnitude() {
        return glm::length2(this->axis);
    }
    
    inline vec3 get_normalized() {
        float mag = this->get_magnitude();
        if (mag == 0.0)
            return vec3(this->axis.x, this->axis.y, this->axis.z);
        return vec3(this->axis.x/mag, this->axis.y/mag, this->axis.z/mag);
    }

    // Quaternion operations:
    vec3 cross(quaternion const& other);

    vec3 operator*(quaternion const& other);
        
};

