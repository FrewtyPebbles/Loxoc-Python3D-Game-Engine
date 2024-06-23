#include "Material.h"
#include "util.h"
#include <sstream>

void material::set_uniform(string name, uniform_type value, string type) {
    int loc = glGetUniformLocation(this->shader_program, name.c_str());
    type = str_tool::to_lowercase(type);
    
    if (type == "f" || type == "float" || type == "i" || type == "int") {
        this->uniforms.insert_or_assign(loc, value);
    } else {
        std::vector<float> var_vec = std::get<std::vector<float>>(value);
        if (type == "v2" || type == "vec2") {
            this->uniforms.insert_or_assign(loc, glm::vec2(var_vec[0], var_vec[1]));
        } else if (type == "v3" || type == "vec3") {
            this->uniforms.insert_or_assign(loc, glm::vec3(var_vec[0], var_vec[1], var_vec[2]));
        } else if (type == "v4" || type == "vec4") {
            this->uniforms.insert_or_assign(loc, glm::vec4(var_vec[0], var_vec[1], var_vec[2], var_vec[3]));
        } else if (type == "m2" || type == "mat2") {
            this->uniforms.insert_or_assign(loc, glm::mat2(var_vec[0], var_vec[1], var_vec[2], var_vec[3]));
        } else if (type == "m3" || type == "mat3") {
            this->uniforms.insert_or_assign(loc, glm::mat3(
                var_vec[0], var_vec[1], var_vec[2],
                var_vec[3], var_vec[4], var_vec[5],
                var_vec[6], var_vec[7], var_vec[8]
            ));
        } else if (type == "m4" || type == "mat4") {
            this->uniforms.insert_or_assign(loc, glm::mat4(
                var_vec[0], var_vec[1], var_vec[2], var_vec[3],
                var_vec[4], var_vec[5], var_vec[6], var_vec[7],
                var_vec[8], var_vec[9], var_vec[10], var_vec[11],
                var_vec[12], var_vec[13], var_vec[14], var_vec[15]
            ));
        } else if (type == "m2x3" || type == "mat2x3") {
            this->uniforms.insert_or_assign(loc, glm::mat2x3(
                var_vec[0], var_vec[1],
                var_vec[2], var_vec[3],
                var_vec[4], var_vec[5]
            ));
        } else if (type == "m3x2" || type == "mat3x2") {
            this->uniforms.insert_or_assign(loc, glm::mat3x2(
                var_vec[0], var_vec[1],
                var_vec[2], var_vec[3],
                var_vec[4], var_vec[5]
            ));
        } else if (type == "m2x4" || type == "mat2x4") {
            this->uniforms.insert_or_assign(loc, glm::mat2x4(
                var_vec[0], var_vec[1],
                var_vec[2], var_vec[3],
                var_vec[4], var_vec[5],
                var_vec[6], var_vec[7]
            ));
        } else if (type == "m4x2" || type == "mat4x2") {
            this->uniforms.insert_or_assign(loc, glm::mat4x2(
                var_vec[0], var_vec[1],
                var_vec[2], var_vec[3],
                var_vec[4], var_vec[5],
                var_vec[6], var_vec[7]
            ));
        } else if (type == "m3x4" || type == "mat3x4") {
            this->uniforms.insert_or_assign(loc, glm::mat3x4(
                var_vec[0], var_vec[1], var_vec[2],
                var_vec[3], var_vec[4], var_vec[5],
                var_vec[6], var_vec[7], var_vec[8],
                var_vec[9], var_vec[10], var_vec[11]
            ));
        } else if (type == "m4x3" || type == "mat4x3") {
            this->uniforms.insert_or_assign(loc, glm::mat4x3(
                var_vec[0], var_vec[1], var_vec[2],
                var_vec[3], var_vec[4], var_vec[5],
                var_vec[6], var_vec[7], var_vec[8],
                var_vec[9], var_vec[10], var_vec[11]
            ));
        } else if (type == "quat" || type == "quat") {
            this->uniforms.insert_or_assign(loc, glm::quat(
                var_vec[0], var_vec[1],
                var_vec[2], var_vec[3]
            ));
        }
    }
}

void material::use_material() {
    glUseProgram(this->shader_program);
}

void material::register_uniforms() {
    for (auto [loc, value] : this->uniforms) {
        if (std::holds_alternative<GLfloat>(value)) {
            glUniform1f(loc, std::get<GLfloat>(value));
        } else if (std::holds_alternative<GLint>(value)) {
            glUniform1i(loc, std::get<GLint>(value));
        } else if (std::holds_alternative<GLuint>(value)) {
            glUniform1ui(loc, std::get<GLuint>(value));
        } else if (std::holds_alternative<glm::vec2>(value)) {
            glUniform2fv(loc, 1, glm::value_ptr(std::get<glm::vec2>(value)));
        } else if (std::holds_alternative<glm::vec3>(value)) {
            glUniform3fv(loc, 1, glm::value_ptr(std::get<glm::vec3>(value)));
        } else if (std::holds_alternative<glm::vec4>(value)) {
            glUniform4fv(loc, 1, glm::value_ptr(std::get<glm::vec4>(value)));
        } else if (std::holds_alternative<glm::mat2>(value)) {
            glUniformMatrix2fv(loc, 1, GL_FALSE, glm::value_ptr(std::get<glm::mat2>(value)));
        } else if (std::holds_alternative<glm::mat3>(value)) {
            glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(std::get<glm::mat3>(value)));
        } else if (std::holds_alternative<glm::mat4>(value)) {
            glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(std::get<glm::mat4>(value)));
        } else if (std::holds_alternative<glm::mat2x3>(value)) {
            glUniformMatrix2x3fv(loc, 1, GL_FALSE, glm::value_ptr(std::get<glm::mat2x3>(value)));
        } else if (std::holds_alternative<glm::mat3x2>(value)) {
            glUniformMatrix3x2fv(loc, 1, GL_FALSE, glm::value_ptr(std::get<glm::mat3x2>(value)));
        } else if (std::holds_alternative<glm::mat2x4>(value)) {
            glUniformMatrix2x4fv(loc, 1, GL_FALSE, glm::value_ptr(std::get<glm::mat2x4>(value)));
        } else if (std::holds_alternative<glm::mat4x2>(value)) {
            glUniformMatrix4x2fv(loc, 1, GL_FALSE, glm::value_ptr(std::get<glm::mat4x2>(value)));
        } else if (std::holds_alternative<glm::mat3x4>(value)) {
            glUniformMatrix3x4fv(loc, 1, GL_FALSE, glm::value_ptr(std::get<glm::mat3x4>(value)));
        } else if (std::holds_alternative<glm::mat4x3>(value)) {
            glUniformMatrix4x3fv(loc, 1, GL_FALSE, glm::value_ptr(std::get<glm::mat4x3>(value)));
        } else if (std::holds_alternative<glm::quat>(value)) {
            glUniform4fv(loc, 1, glm::value_ptr(std::get<glm::quat>(value)));
        }
    }
} 

void material::link_shaders() {
    this->vertex->compile();
    this->fragment->compile();
    this->shader_program = glCreateProgram();
    glAttachShader(this->shader_program, this->vertex->shader_handle);
    glAttachShader(this->shader_program, this->fragment->shader_handle);
    glLinkProgram(this->shader_program);
    
    // Check for linking errors
    GLint success;
    GLchar infoLog[512];
    glGetProgramiv(this->shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(this->shader_program, 512, NULL, infoLog);
        std::stringstream ss;
        ss << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << "\n";
        throw std::runtime_error(ss.str());
    }
    // glDetachShader(this->shader_program, this->vertex_shader.shader_handle);
    // glDetachShader(this->shader_program, this->fragment_shader.shader_handle);
    glDeleteShader(this->vertex->shader_handle);
    glDeleteShader(this->fragment->shader_handle);
}