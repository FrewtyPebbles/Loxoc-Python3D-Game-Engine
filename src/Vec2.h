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
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include "Quaternion.h"

// cpp class names are camelcase, python class names are title case
typedef glm::vec2 glmvec2;

class camera;

class vec2 {
public:
    vec2()=default;
    vec2(float x, float y) : axis(glm::vec2(x,y)) {}
    vec2(const glm::vec2& axis) : axis(axis) {}
    vec2(const vec2& rhs) : axis(rhs.axis) {}
    glm::vec2 axis;

    friend inline std::ostream& operator<<(std::ostream& os, const vec2& self){
        os << "vec2<" << self.axis.x << ", " << self.axis.y << '>';
        return os;
    }

    float get_x() {return axis.x;}
    float get_y() {return axis.y;}

    void set_x(float other) {axis.x = other;}
    void set_y(float other) {axis.y = other;}



    // Operators

    inline vec2 operator-() {
        return vec2(-axis.x, -axis.y);
    }

    inline vec2 operator+(vec2 const& other)
    {
        
        return vec2(axis.x+other.axis.x, axis.y+other.axis.y);
    }

    inline vec2 operator+(float const& other)
    {
        return vec2(axis.x+other, axis.y+other);
    }

    inline vec2 operator-(vec2 const& other)
    {
        return vec2(axis.x-other.axis.x, axis.y-other.axis.y);
    }

    inline vec2 operator-(float const& other)
    {
        return vec2(axis.x-other, axis.y-other);
    }
    
    inline vec2 operator*(vec2 const& other)
    {
        return vec2(axis.x*other.axis.x, axis.y*other.axis.y);
    }

    inline vec2 operator*(float const& other)
    {
        return vec2(axis.x*other, axis.y*other);
    }
    
    inline vec2 operator/(vec2 const& other)
    {
        return vec2(axis.x/other.axis.x, axis.y/other.axis.y);
    }

    inline vec2 operator/(float const& other)
    {
        return vec2(axis.x/other, axis.y/other);
    }

    inline float dot(vec2 const& other)
    {
        return glm::dot(this->axis, other.axis);
    }

    inline float get_magnitude() {
        return glm::length2(this->axis);
    }
    
    inline vec2 get_normalized() {
        return vec2(glm::normalize(this->axis));
    }

    inline float to_angle() {
        return std::atan2(this->axis.y, this->axis.x);
    }

    static inline vec2 from_angle(float angle) {
        return vec2(cos(angle), sin(angle));
    }
        
};

