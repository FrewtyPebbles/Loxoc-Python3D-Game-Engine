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

// TODO: add lerp operation to python frontend

#define xyz_comp(sign) self.axis.x sign other.axis.x && self.axis.y sign other.axis.y && self.axis.z sign other.axis.z

// cpp class names are camelcase, python class names are title case
typedef glm::vec3 glmvec3;

class camera;
class quaternion;

class vec2;
class vec4;

template<typename glm_mat_type>
class matrix;

class vec3 {
public:
    vec3()=default;
    vec3(float x, float y, float z) : axis(x,y,z) {}
    vec3(float val) : axis(val,val,val) {}
    vec3(const glm::vec3& axis) : axis(axis) {}
    vec3(const vec3& rhs) : axis(rhs.axis) {}
    vec3(const vec2& rhs, float z);
    vec3(const vec4& rhs);
    glm::vec3 axis;

    friend inline std::strong_ordering operator<=>(const vec3& self, const vec3& other) {
        if (xyz_comp(>))
            return std::strong_ordering::greater;
        if (xyz_comp(<))
            return std::strong_ordering::less;
        return std::strong_ordering::equal;
    }

    friend inline bool operator==(const vec3& lhs, const vec3& rhs) {
        return lhs.axis.x == rhs.axis.x && lhs.axis.y == rhs.axis.y && lhs.axis.z == rhs.axis.z;
    }
    
    friend inline bool operator<=(const vec3& lhs, const vec3& rhs) {
        return lhs.axis.x <= rhs.axis.x && lhs.axis.y <= rhs.axis.y && lhs.axis.z <= rhs.axis.z;
    }

    friend inline bool operator>=(const vec3& lhs, const vec3& rhs) {
        return lhs.axis.x >= rhs.axis.x && lhs.axis.y >= rhs.axis.y && lhs.axis.z >= rhs.axis.z;
    }

    friend inline bool operator<(const vec3& lhs, const vec3& rhs) {
        return lhs.axis.x < rhs.axis.x && lhs.axis.y < rhs.axis.y && lhs.axis.z < rhs.axis.z;
    }

    friend inline bool operator>(const vec3& lhs, const vec3& rhs) {
        return lhs.axis.x > rhs.axis.x && lhs.axis.y > rhs.axis.y && lhs.axis.z > rhs.axis.z;
    }

    friend inline std::ostream& operator<<(std::ostream& os, const vec3& self){
        os << "vec3<" << self.axis.x << ", " << self.axis.y << ", " << self.axis.z << '>';
        return os;
    }

    float get_x() {return axis.x;}
    float get_y() {return axis.y;}
    float get_z() {return axis.z;}
    void set_x(float other) {axis.x = other;}
    void set_y(float other) {axis.y = other;}
    void set_z(float other) {axis.z = other;}

    inline vec3 get_up() {
        return glm::quat(axis) * glm::vec3(0.0f, 1.0f, 0.0f);
    }

    inline vec3 get_right() {
        return glm::quat(axis) * glm::vec3(1.0f, 0.0f, 0.0f);
    }

    inline vec3 get_forward() {
        return glm::quat(axis) * glm::vec3(0.0f, 0.0f, 1.0f);
    }

    // Operators

    const float& operator[](int index) const {
        return axis[index];
    }

    inline vec3 operator-() {
        return -axis;
    }

    inline vec3 operator+(vec3 const& other)
    {
        
        return axis + other.axis;
    }

    inline vec3 operator+(float const& other)
    {
        return axis + other;
    }

    inline vec3 operator+=(vec3 const& other)
    {
        axis = axis + other.axis;
        return *this;
    }

    inline vec3 operator+=(float const& other)
    {
        axis = axis + other;
        return *this;
    }

    inline vec3 operator-(vec3 const& other)
    {
        return vec3(axis - other.axis);
    }

    inline vec3 operator-(float const& other)
    {
        return axis - other;
    }

    inline vec3 operator-=(vec3 const& other)
    {
        axis = axis - other.axis;
        return *this;
    }

    inline vec3 operator-=(float const& other)
    {
        axis = axis - other;
        return *this;
    }
    
    inline vec3 operator*(vec3 const& other)
    {
        return axis * other.axis;
    }

    inline vec3 operator*(float const& other)
    {
        return axis * other;
    }
    
    inline vec3 operator/(vec3 const& other)
    {
        return axis / other.axis;
    }

    inline vec3 operator/(float const& other)
    {
        return axis / other;
    }

    inline float dot(const vec3& other)
    {
        return glm::dot(this->axis, other.axis);
    }

    inline vec3 cross(vec3 other)
    {
        return glm::cross(this->axis, other.axis);
    }

    inline float get_magnitude() {
        return glm::length(this->axis);
    }
    
    inline vec3 get_normalized() {
        return glm::normalize(this->axis);
    }

    inline float distance(vec3 const& other) {
        return glm::distance(this->axis, other.axis);
    }

    inline vec3 lerp(vec3 const& other, float ratio) {
        return glm::mix(this->axis, other.axis, ratio);
    }

    // Quaternion operations:

    quaternion to_quaternion();

    vec3 cross(quaternion const& other);

    vec3 operator*(quaternion const& other);

    //CONST OPERATIONS

    inline vec3 get_up() const {
        return glm::quat(axis) * glm::vec3(0.0f, 1.0f, 0.0f);
    }

    inline vec3 get_right() const {
        return glm::quat(axis) * glm::vec3(1.0f, 0.0f, 0.0f);
    }

    inline vec3 get_forward() const {
        return glm::quat(axis) * glm::vec3(0.0f, 0.0f, 1.0f);
    }

    // Operators

    inline vec3 operator-() const {
        return -axis;
    }

    inline vec3 operator+(vec3 const& other) const
    {
        return axis + other.axis;
    }

    inline vec3 operator+(float const& other) const
    {
        return axis + other;
    }

    inline vec3 operator-(vec3 const& other) const
    {
        return axis - other.axis;
    }

    inline vec3 operator-(float const& other) const
    {
        return axis - other;
    }
    
    inline vec3 operator*(vec3 const& other) const
    {
        return axis * other.axis;
    }

    inline vec3 operator*(float const& other) const
    {
        return axis * other;
    }
    
    inline vec3 operator/(vec3 const& other) const
    {
        return axis / other.axis;
    }

    inline vec3 operator/(float const& other) const
    {
        return axis / other;
    }

    inline float dot(const vec3& other) const
    {
        return glm::dot(this->axis, other.axis);
    }

    inline vec3 cross(const vec3& other) const
    {
        return glm::cross(this->axis, other.axis);
    }

    inline float get_magnitude() const {
        return glm::length(this->axis);
    }
    
    inline vec3 get_normalized() const {
        return glm::normalize(this->axis);
    }

    inline float distance(vec3 const& other) const {
        return glm::distance(this->axis, other.axis);
    }

    matrix<glm::mat2x3> outer_product(const vec2&);
    matrix<glm::mat3x3> outer_product(const vec3&);
    matrix<glm::mat4x3> outer_product(const vec4&);

    // Quaternion operations:

    quaternion to_quaternion() const;

    vec3 cross(quaternion const& other) const;

    vec3 operator*(quaternion const& other) const;

    // conversions:

    vec4 to_vec4(float w);

    vec2 to_vec2();
        
};

