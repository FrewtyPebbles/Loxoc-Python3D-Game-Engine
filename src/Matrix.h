#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_SSE42 // or GLM_FORCE_SSE42 if your processor supports it
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <compare>
#include "Vec2.h"
#include "Vec3.h"
#include "Vec4.h"

class quaternion;

template<typename T>
using mat_ret_deduce = 
    // if
    std::conditional_t<
    std::is_same_v<T, glm::mat2x2> || std::is_same_v<T, glm::mat3x2> || std::is_same_v<T, glm::mat4x2>,
    vec2,
    // elif
    std::conditional_t<
    std::is_same_v<T, glm::mat2x3> || std::is_same_v<T, glm::mat3x3> || std::is_same_v<T, glm::mat4x3>,
    vec3,
    // elif
    std::conditional_t<
    std::is_same_v<T, glm::mat2x4> || std::is_same_v<T, glm::mat3x4> || std::is_same_v<T, glm::mat4x4>,
    vec4,
    // else
    void
>>>;

template<typename glm_mat_type>
class matrix {
public:
    matrix()=default;
    matrix(float value) : mat(value) {}
    matrix(float, float, float, float);
    matrix(float, float, float, float, float, float);
    matrix(float, float, float, float, float, float, float, float);
    matrix(float, float, float, float, float, float, float, float, float);
    matrix(float, float, float, float, float, float, float, float, float, float, float, float);
    matrix(float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float);
    matrix(const quaternion& quat);
    matrix(quaternion * quat);
    matrix(const glm_mat_type& glm_mat) : mat(glm_mat) {}
    matrix(const matrix<glm_mat_type>& other) : mat(other.mat) {}

    glm_mat_type mat;

    vec3 get_up();

    vec3 get_right();

    vec3 get_forward();

    // Operators

    template<typename mat_type = glm_mat_type>
    inline auto operator[](int index) const -> mat_ret_deduce<mat_type> {
        return mat[index];
    }

    inline vec4 get_vec4(int index) const {
        return mat[index];
    }

    inline vec3 get_vec3(int index) const {
        return mat[index];
    }

    inline vec2 get_vec2(int index) const {
        return mat[index];
    }


    inline matrix<glm_mat_type> operator-() {
        return -mat;
    }

    inline matrix<glm_mat_type> operator+(matrix<glm_mat_type> const& other)
    {
        
        return mat + other.mat;
    }

    inline matrix<glm_mat_type> operator+(float const& other)
    {
        return mat + other;
    }

    inline matrix<glm_mat_type> operator+=(matrix<glm_mat_type> const& other) {
        mat += other.mat;
        return *this;
    }

    inline matrix<glm_mat_type> operator-(matrix<glm_mat_type> const& other)
    {
        return mat - other.mat;
    }

    inline matrix<glm_mat_type> operator-(float const& other)
    {
        return mat - other;
    }

    inline matrix<glm_mat_type> operator-=(matrix<glm_mat_type> const& other) {
        mat -= other.mat;
        return *this;
    }
    
    inline matrix<glm_mat_type> operator*(matrix<glm_mat_type> const& other)
    {
        return mat * other.mat;
    }

    inline matrix<glm_mat_type> operator*=(matrix<glm_mat_type> const& other) {
        mat *= other.mat;
        return *this;
    }

    inline vec2 operator*(const vec2& other) {
        return mat * other.axis;
    }
    
    inline vec3 operator*(const vec3& other) {
        return mat * other.axis;
    }

    inline vec3 mul_4x3(const vec4& other) {
        return mat * other.axis;
    }

    inline vec3 mul_2x3(const vec2& other) {
        return mat * other.axis;
    }

    inline vec2 mul_3x2(const vec3& other) {
        return mat * other.axis;
    }

    inline vec2 mul_4x2(const vec4& other) {
        return mat * other.axis;
    }

    inline vec4 operator*(const vec4& other) {
        return mat * other.axis;
    }

    inline matrix<glm_mat_type> operator*(float const& other)
    {
        return mat * other;
    }

    inline matrix<glm_mat_type> operator=(matrix<glm_mat_type> const& other)
    {
        mat = other.mat;
        return *this;
    }
    
    inline matrix<glm_mat_type> operator/(matrix<glm_mat_type> const& other)
    {
        glm::mat3x4() * glm::vec4();
        return mat / other.mat;
    }

    inline matrix<glm_mat_type> operator/(float const& other)
    {
        return mat / other;
    }

    inline matrix<glm_mat_type> operator/=(matrix<glm_mat_type> const& other) {
        mat /= other.mat;
        return *this;
    }

    inline matrix<glm_mat_type> translate(const vec3& vec) {
        return glm::translate(mat, vec.axis);
    }

    inline matrix<glm_mat_type> translate(vec3 * vec) {
        return glm::translate(mat, vec->axis);
    }

    inline matrix<glm_mat_type> rotate(float angle, const vec3& axis) {
        return glm::rotate(mat, angle, axis.axis);
    }

    inline matrix<glm_mat_type> rotate(float angle, vec3 * axis) {
        return glm::rotate(mat, angle, axis->axis);
    }

    inline matrix<glm_mat_type> scale(const vec3& vec) {
        return glm::scale(mat, vec.axis);
    }
    
    inline matrix<glm_mat_type> scale(vec3 * vec) {
        return glm::scale(mat, vec->axis);
    }

    inline matrix<glm_mat_type> scale(const vec2& vec) {
        return glm::scale(mat, vec.axis);
    }
    
    inline matrix<glm_mat_type> scale(vec2 * vec) {
        return glm::scale(mat, vec->axis);
    }

    inline matrix<glm_mat_type> inverse() {
        return glm::inverse(this->mat);
    }

    inline float determinant() {
        return glm::determinant(this->mat);
    }

    // Quaternion operations:

    quaternion to_quaternion();

};
//glm mats
typedef glm::mat2x2 glmmat2x2;
typedef glm::mat2x3 glmmat2x3;
typedef glm::mat2x4 glmmat2x4;

typedef glm::mat3x2 glmmat3x2;
typedef glm::mat3x3 glmmat3x3;
typedef glm::mat3x4 glmmat3x4;

typedef glm::mat4x2 glmmat4x2;
typedef glm::mat4x3 glmmat4x3;
typedef glm::mat4x4 glmmat4x4;

//mats
typedef matrix<glm::mat2x2> matrix2x2;
typedef matrix<glm::mat2x3> matrix2x3;
typedef matrix<glm::mat2x4> matrix2x4;

typedef matrix<glm::mat3x2> matrix3x2;
typedef matrix<glm::mat3x3> matrix3x3;
typedef matrix<glm::mat3x4> matrix3x4;

typedef matrix<glm::mat4x2> matrix4x2;
typedef matrix<glm::mat4x3> matrix4x3;
typedef matrix<glm::mat4x4> matrix4x4;