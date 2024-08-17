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

#define xyz_comp(sign) self.axis.x sign other.axis.x && self.axis.y sign other.axis.y && self.axis.z sign other.axis.z

// cpp class names are camelcase, python class names are title case
typedef glm::vec4 glmvec4;

class camera;
class quaternion;

class vec2;

template<typename glm_mat_type>
class matrix;

class vec4 {
public:
    vec4()=default;
    vec4(float x, float y, float z, float w) : axis(x,y,z,w) {}
    vec4(float val) : axis(val,val,val,val) {}
    vec4(const glm::vec4& axis) : axis(axis) {}
    vec4(const vec4& rhs) : axis(rhs.axis) {}
    vec4(const vec3& rhs, float w);
    vec4(const vec2& rhs, float z, float w);
    glm::vec4 axis;

    friend inline std::strong_ordering operator<=>(const vec4& self, const vec4& other) {
        if (xyz_comp(>))
            return std::strong_ordering::greater;
        if (xyz_comp(<))
            return std::strong_ordering::less;
        return std::strong_ordering::equal;
    }
    
    
    friend inline bool operator<=(const vec4& lhs, const vec4& rhs) {
        return lhs.axis.x <= rhs.axis.x && lhs.axis.y <= rhs.axis.y && lhs.axis.z <= rhs.axis.z && lhs.axis.w <= rhs.axis.w;
    }

    friend inline bool operator>=(const vec4& lhs, const vec4& rhs) {
        return lhs.axis.x >= rhs.axis.x && lhs.axis.y >= rhs.axis.y && lhs.axis.z >= rhs.axis.z && lhs.axis.w >= rhs.axis.w;
    }

    friend inline bool operator<(const vec4& lhs, const vec4& rhs) {
        return lhs.axis.x < rhs.axis.x && lhs.axis.y < rhs.axis.y && lhs.axis.z < rhs.axis.z && lhs.axis.w < rhs.axis.w;
    }

    friend inline bool operator>(const vec4& lhs, const vec4& rhs) {
        return lhs.axis.x > rhs.axis.x && lhs.axis.y > rhs.axis.y && lhs.axis.z > rhs.axis.z && lhs.axis.w > rhs.axis.w;
    }

    friend inline std::ostream& operator<<(std::ostream& os, const vec4& self){
        os << "vec4<" << self.axis.x << ", " << self.axis.y << ", " << self.axis.z << ", " << self.axis.w << '>';
        return os;
    }

    float get_x() {return axis.x;}
    float get_y() {return axis.y;}
    float get_z() {return axis.z;}
    float get_w() {return axis.w;}
    void set_x(float other) {axis.x = other;}
    void set_y(float other) {axis.y = other;}
    void set_z(float other) {axis.z = other;}
    void set_w(float other) {axis.w = other;}

    inline vec4 get_up() {
        return glm::quat(axis) * glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
    }

    inline vec4 get_right() {
        return glm::quat(axis) * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    }

    inline vec4 get_forward() {
        return glm::quat(axis) * glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
    }

    // Operators

    const float& operator[](int index) const {
        return axis[index];
    }

    inline vec4 operator-() {
        return -axis;
    }

    inline vec4 operator+(vec4 const& other)
    {
        
        return axis + other.axis;
    }

    inline vec4 operator+(float const& other)
    {
        return axis + other;
    }

    inline vec4 operator-(vec4 const& other)
    {
        return axis - other.axis;
    }

    inline vec4 operator-(float const& other)
    {
        return axis - other;
    }
    
    inline vec4 operator*(vec4 const& other)
    {
        return axis * other.axis;
    }

    inline vec4 operator*(float const& other)
    {
        return axis * other;
    }
    
    inline vec4 operator/(vec4 const& other)
    {
        return axis / other.axis;
    }

    inline vec4 operator/(float const& other)
    {
        return axis / other;
    }

    inline float dot(const vec4& other)
    {
        return glm::dot(this->axis, other.axis);
    }

    inline float get_magnitude() {
        return glm::length(this->axis);
    }
    
    inline vec4 get_normalized() {
        return glm::normalize(this->axis);
    }

    inline float distance(vec4 const& other) {
        return glm::distance(this->axis, other.axis);
    }

    // Quaternion operations:

    vec4 operator*(quaternion const& other);

    //CONST OPERATIONS

    inline vec4 get_up() const {
        return glm::quat(axis) * glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
    }

    inline vec4 get_right() const {
        return glm::quat(axis) * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    }

    inline vec4 get_forward() const {
        return glm::quat(axis) * glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
    }

    // Operators

    inline vec4 operator-() const {
        return -axis;
    }

    inline vec4 operator+(vec4 const& other) const
    {
        
        return axis + other.axis;
    }

    inline vec4 operator+(float const& other) const
    {
        return axis + other;
    }

    inline vec4 operator-(vec4 const& other) const
    {
        return axis - other.axis;
    }

    inline vec4 operator-(float const& other) const
    {
        return axis - other;
    }
    
    inline vec4 operator*(vec4 const& other) const
    {
        return axis * other.axis;
    }

    inline vec4 operator*(float const& other) const
    {
        return axis * other;
    }
    
    inline vec4 operator/(vec4 const& other) const
    {
        return axis / other.axis;
    }

    inline vec4 operator/(float const& other) const
    {
        return axis / other;
    }

    inline float dot(const vec4& other) const
    {
        return glm::dot(this->axis, other.axis);
    }

    inline float get_magnitude() const {
        return glm::length(this->axis);
    }
    
    inline vec4 get_normalized() const {
        return glm::normalize(this->axis);
    }

    inline float distance(vec4 const& other) const {
        return glm::distance(this->axis, other.axis);
    }

    matrix<glm::mat2x4> outer_product(const vec2&);
    matrix<glm::mat3x4> outer_product(const vec3&);
    matrix<glm::mat4x4> outer_product(const vec4&);

    // Quaternion operations:

    vec4 operator*(quaternion const& other) const;

    // CONVERSIONS

    vec3 to_vec3();
    vec2 to_vec2();
};

