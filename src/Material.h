#pragma once
#include "Shader.h"
#include <map>
#include <variant>
#include "glad/gl.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>

using std::string;
using std::map;

typedef std::variant<
    glm::vec2,
    glm::vec3,
    glm::vec4,
    GLfloat,
    GLint,
    GLuint,
    glm::mat2,
    glm::mat3,
    glm::mat4,
    glm::mat2x3,
    glm::mat3x2,
    glm::mat2x4,
    glm::mat4x2,
    glm::mat3x4,
    glm::mat4x3,
    glm::quat,
    std::vector<float>
> uniform_type;

class TRAIT_has_uniform {
public:
    map<int, uniform_type> uniforms;
    
    void register_uniforms();
protected:
    void inner_set_uniform(int loc, string name, uniform_type value, string type);
};

class material : public TRAIT_has_uniform {
public:
    material(shader* vertex, shader* fragment)
    : vertex(vertex), fragment(fragment)
    {
        this->link_shaders();
    }
    ~material(){}
    void set_uniform(string name, uniform_type value, string type);
    void link_shaders();

    // calls use shader program
    void use_material();

    shader* vertex;
    shader* fragment;
    GLuint shader_program;
};