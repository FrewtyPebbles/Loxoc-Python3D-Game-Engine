#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include <iostream>

// TODO: add slerp operation to python frontend

class vec3;

typedef glm::quat glmquat;

class quaternion {
public:
    quaternion(){}
    quaternion(float w, float x, float y, float z){
        this->quat = glm::quat(w, x, y, z);
    }
    quaternion(const glm::quat& quat) : quat(quat) {}
    quaternion(const quaternion& q) : quat(q.quat) {}
    // Euler Angles in Radians
    vec3 to_euler();
    // From Euler Angles in Radians
    static quaternion from_euler(const vec3& euler_vec);
    static quaternion from_axis_angle(vec3 axis, float angle);

    void rotate(vec3 axis, float angle);

    glm::quat quat; 

    float get_w() {return quat.w;}
    float get_x() {return quat.x;}
    float get_y() {return quat.y;}
    float get_z() {return quat.z;}
    void set_w(float other) {quat.w = other;}
    void set_x(float other) {quat.x = other;}
    void set_y(float other) {quat.y = other;}
    void set_z(float other) {quat.z = other;}

    friend inline std::ostream& operator<<(std::ostream& os, const quaternion& self){
        os << "quat<" << self.quat.w << ", " << self.quat.x << ", " << self.quat.y << ", " << self.quat.z << '>';
        return os;
    }

    vec3 get_up();

    vec3 get_right();

    vec3 get_forward();

    // Operators

    inline quaternion operator-() {
        return quaternion(-quat);
    }

    inline quaternion operator+(quaternion const& other)
    {
        
        return quaternion(quat + other.quat);
    }

    inline quaternion operator+(float const& other)
    {
        glm::quat t_q = quat;
        t_q.w += other;
        t_q.x += other;
        t_q.y += other;
        t_q.z += other;
        return quaternion(t_q);
    }

    inline quaternion operator-(quaternion const& other)
    {
        return quaternion(quat-other.quat);
    }

    inline quaternion operator-(float const& other)
    {
        glm::quat t_q = quat;
        t_q.w -= other;
        t_q.x -= other;
        t_q.y -= other;
        t_q.z -= other;
        return quaternion(t_q);
    }
    
    inline quaternion operator*(quaternion const& other)
    {
        return quaternion(quat*other.quat);
    }

    inline quaternion operator*(float const& other)
    {
        return quaternion(quat*other);
    }
    
    inline quaternion operator/(quaternion const& other)
    {
        return quaternion(quat.w/other.quat.w, quat.x/other.quat.x, quat.y/other.quat.y, quat.z/other.quat.z);
    }

    inline quaternion operator/(float const& other)
    {
        return quaternion(quat/other);
    }

    inline float dot(quaternion const& other)
    {
        return glm::dot(this->quat, other.quat);
    }

    inline quaternion cross(quaternion const& other)
    {
        return quaternion(glm::cross(this->quat, other.quat));
    }

    inline quaternion slerp(quaternion const& other, float ratio)
    {
        return glm::slerp(this->quat, other.quat, ratio);
    }

    inline float get_magnitude() {
        return glm::length2(this->quat);
    }

    inline quaternion get_normalized() {
        return glm::normalize(this->quat);
    }

    inline quaternion lerp(const quaternion& other, float ratio) {
        return glm::lerp(this->quat, other.quat, ratio);
    }

    // Vector operations:
    vec3 cross(vec3 const& other);
    vec3 operator*(vec3 const& other);
};