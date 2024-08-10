#pragma once
#include "Shader.h"
#include <map>
#include <variant>
#include "glad/gl.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>
#include "RC.h"
#include "Vec2.h"
#include "Vec3.h"
#include "Vec4.h"
#include "Matrix.h"
#include "Texture.h"

using std::string;
using std::map;

typedef std::variant< // switch to wrapper types
    vec2,
    vec3,
    vec4,
    GLfloat,
    GLint,
    GLuint,
    matrix2x2,
    matrix3x3,
    matrix4x4,
    matrix2x3,
    matrix3x2,
    matrix2x4,
    matrix4x2,
    matrix3x4,
    matrix4x3,
    glm::quat,
    std::vector<float>
> uniform_type;

class TRAIT_has_uniform {
public:
    map<int, uniform_type> uniforms;
    
    void register_uniforms();
protected:
    void inner_set_uniform(int loc, uniform_type value);
};

typedef RC<texture*>* rc_texture;
typedef RC<shader*>* rc_shader;

class material : public TRAIT_has_uniform {
public:
    
    material(rc_shader vertex, rc_shader fragment, rc_shader geometry = nullptr, rc_shader compute = nullptr)
    : vertex(vertex), fragment(fragment), geometry(geometry), compute(compute)
    {
        this->link_shaders();
    }

    ~material(){}
    void set_uniform(string name, uniform_type value);
    void link_shaders();

    // calls use shader program
    void use_material();

    void set_material();

    rc_shader vertex;
    rc_shader fragment;
    rc_shader geometry;
    rc_shader compute;
    GLuint shader_program;
    string name;
    vec3 ambient = vec3(0.1f, 0.1f, 0.1f);
    vec3 diffuse = vec3(1.0f, 1.0f, 1.0f);
    vec3 specular = vec3(1.0f, 1.0f, 1.0f);
    float shine = 1.0f;
    float specular_exponent, optical_density, transparency;
    rc_texture diffuse_texture = nullptr;
    rc_texture specular_texture = nullptr;
    rc_texture normals_texture = nullptr;
};

typedef RC<material*>* rc_material;