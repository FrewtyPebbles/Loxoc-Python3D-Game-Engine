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
#include <compare>

#define xy_comp(sign)  self.axis.x sign other.axis.x && self.axis.y sign other.axis.y

// cpp class names are camelcase, python class names are title case
typedef glm::vec2 glmvec2;

class camera;
class vec3;
class vec4;

template<typename glm_mat_type>
class matrix;

class vec2 {
public:
    vec2()=default;
    vec2(float x, float y) : axis(x,y) {}
    vec2(float val) : axis(val) {}
    vec2(const glm::vec2& axis) : axis(axis) {}
    vec2(const vec2& rhs) : axis(rhs.axis) {}
    vec2(const vec3& rhs);
    vec2(const vec4& rhs);
    glm::vec2 axis;

    friend inline std::strong_ordering operator<=>(const vec2& self, const vec2& other) {
        if (xy_comp(>))
            return std::strong_ordering::greater;
        if (xy_comp(<))
            return std::strong_ordering::less;
        return std::strong_ordering::equal;
    }

    friend inline bool operator<=(const vec2& lhs, const vec2& rhs) {
        return lhs.axis.x <= rhs.axis.x && lhs.axis.y <= rhs.axis.y;
    }

    friend inline bool operator>=(const vec2& lhs, const vec2& rhs) {
        return lhs.axis.x >= rhs.axis.x && lhs.axis.y >= rhs.axis.y;
    }

    friend inline bool operator<(const vec2& lhs, const vec2& rhs) {
        return lhs.axis.x <= rhs.axis.x && lhs.axis.y <= rhs.axis.y;
    }

    friend inline bool operator>(const vec2& lhs, const vec2& rhs) {
        return lhs.axis.x >= rhs.axis.x && lhs.axis.y >= rhs.axis.y;
    }

    friend inline std::ostream& operator<<(std::ostream& os, const vec2& self){
        os << "vec2<" << self.axis.x << ", " << self.axis.y << '>';
        return os;
    }

    float get_x() {return axis.x;}
    float get_y() {return axis.y;}

    void set_x(float other) {axis.x = other;}
    void set_y(float other) {axis.y = other;}



    // Operators

    const float& operator[](int index) const {
        return axis[index];
    }

    inline vec2 operator-() {
        return -axis;
    }

    inline vec2 operator+(vec2 const& other)
    {
        
        return axis + other.axis;
    }

    inline vec2 operator+(float const& other)
    {
        return axis + other;
    }

    inline vec2 operator-(vec2 const& other)
    {
        return axis - other.axis;
    }

    inline vec2 operator-(float const& other)
    {
        return axis - other;
    }
    
    inline vec2 operator*(vec2 const& other)
    {
        return axis * other.axis;
    }

    inline vec2 operator*(float const& other)
    {
        return axis * other;
    }
    
    inline vec2 operator/(vec2 const& other)
    {
        return axis / other.axis;
    }

    inline vec2 operator/(float const& other)
    {
        return axis / other;
    }

    inline float dot(vec2 const& other)
    {
        return glm::dot(this->axis, other.axis);
    }

    inline vec2 lerp(vec2 const& other, float ratio) {
        return glm::mix(this->axis, other.axis, ratio);
    }

    inline float get_magnitude() {
        return glm::length(this->axis);
    }
    
    inline vec2 get_normalized() {
        return glm::normalize(this->axis);
    }

    inline float to_angle() {
        return std::atan2(this->axis.y, this->axis.x);
    }

    static inline vec2 from_angle(float angle) {
        glm::outerProduct(glm::vec2(1.0f), glm::vec3(1.0f));
        return vec2(cos(angle), sin(angle));
    }

    matrix<glm::mat2x2> outer_product(const vec2&);
    matrix<glm::mat3x2> outer_product(const vec3&);
    matrix<glm::mat4x2> outer_product(const vec4&);
    
    // Conversions

    vec4 to_vec4(float z, float w);
    vec3 to_vec3(float z);
};

