#include "Material.h"
#include "util.h"
#include <sstream>
#include "Texture.h"
#include "Object3d.h"

void material::set_uniform(string name, uniform_type value) {
    int loc = glGetUniformLocation(this->shader_program, name.c_str());
    this->inner_set_uniform(loc, value);
}
 
void material::use_material() {
    glUseProgram(this->shader_program);
}

void TRAIT_has_uniform::inner_set_uniform(int loc, uniform_type value) {
    this->uniforms.insert_or_assign(loc, value);
}

void TRAIT_has_uniform::register_uniforms() {
    for (auto [loc, value] : this->uniforms) {
        if (std::holds_alternative<GLfloat>(value)) {
            glUniform1f(loc, std::get<GLfloat>(value));
        } else if (std::holds_alternative<GLint>(value)) {
            glUniform1i(loc, std::get<GLint>(value));
        } else if (std::holds_alternative<GLuint>(value)) {
            glUniform1ui(loc, std::get<GLuint>(value));
        } else if (std::holds_alternative<vec2>(value)) {
            glUniform2fv(loc, 1, glm::value_ptr(std::get<vec2>(value).axis));
        } else if (std::holds_alternative<vec3>(value)) {
            glUniform3fv(loc, 1, glm::value_ptr(std::get<vec3>(value).axis));
        } else if (std::holds_alternative<vec4>(value)) {
            glUniform4fv(loc, 1, glm::value_ptr(std::get<vec4>(value).axis));
        } else if (std::holds_alternative<matrix2x2>(value)) {
            glUniformMatrix2fv(loc, 1, GL_FALSE, glm::value_ptr(std::get<matrix2x2>(value).mat));
        } else if (std::holds_alternative<matrix3x3>(value)) {
            glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(std::get<matrix3x3>(value).mat));
        } else if (std::holds_alternative<matrix4x4>(value)) {
            glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(std::get<matrix4x4>(value).mat));
        } else if (std::holds_alternative<matrix2x3>(value)) {
            glUniformMatrix2x3fv(loc, 1, GL_FALSE, glm::value_ptr(std::get<matrix2x3>(value).mat));
        } else if (std::holds_alternative<matrix3x2>(value)) {
            glUniformMatrix3x2fv(loc, 1, GL_FALSE, glm::value_ptr(std::get<matrix3x2>(value).mat));
        } else if (std::holds_alternative<matrix2x4>(value)) {
            glUniformMatrix2x4fv(loc, 1, GL_FALSE, glm::value_ptr(std::get<matrix2x4>(value).mat));
        } else if (std::holds_alternative<matrix4x2>(value)) {
            glUniformMatrix4x2fv(loc, 1, GL_FALSE, glm::value_ptr(std::get<matrix4x2>(value).mat));
        } else if (std::holds_alternative<matrix3x4>(value)) {
            glUniformMatrix3x4fv(loc, 1, GL_FALSE, glm::value_ptr(std::get<matrix3x4>(value).mat));
        } else if (std::holds_alternative<matrix4x3>(value)) {
            glUniformMatrix4x3fv(loc, 1, GL_FALSE, glm::value_ptr(std::get<matrix4x3>(value).mat));
        } else if (std::holds_alternative<glm::quat>(value)) {
            glUniform4fv(loc, 1, glm::value_ptr(std::get<glm::quat>(value)));
        }
    }
} 

void material::link_shaders() {
    this->vertex->data->compile();
    this->fragment->data->compile();
    if (this->geometry)
        this->geometry->data->compile();
    if (this->compute)
        this->compute->data->compile();
    this->shader_program = glCreateProgram();
    glAttachShader(this->shader_program, this->vertex->data->shader_handle);
    glAttachShader(this->shader_program, this->fragment->data->shader_handle);
    if (this->geometry)
        glAttachShader(this->shader_program, this->geometry->data->shader_handle);
    if (this->compute)
        glAttachShader(this->shader_program, this->compute->data->shader_handle);
    
    glLinkProgram(this->shader_program);
    
    // Check for linking errors
    GLint success;
    GLchar infoLog[512];
    glGetProgramiv(this->shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(this->shader_program, 512, NULL, infoLog);
        std::stringstream ss;
        ss << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << "\n";
        std::cerr << ss.str();
        throw std::runtime_error(ss.str());
    }
    // glDetachShader(this->shader_program, this->vertex_shader.shader_handle);
    // glDetachShader(this->shader_program, this->fragment_shader.shader_handle);
    glDeleteShader(this->vertex->data->shader_handle);
    glDeleteShader(this->fragment->data->shader_handle);
    if (this->geometry)
        glDeleteShader(this->geometry->data->shader_handle);
    if (this->compute)
        glDeleteShader(this->compute->data->shader_handle);
}

void material::set_material() {
    string prefix = "material.";
    // set struct parameters
    
    GLuint ambient_loc = glGetUniformLocation(this->shader_program, (prefix + "ambient").c_str());
    glUniform3fv(ambient_loc, 1, glm::value_ptr(this->ambient.axis));
    
    if (this->diffuse_texture) {
        glActiveTexture(GL_TEX_N_ITTER[0]);
        this->diffuse_texture->data->bind();
    }
    
    if (this->specular_texture != nullptr) {
        glActiveTexture(GL_TEX_N_ITTER[1]);
        this->specular_texture->data->bind();
    }
    
    
    GLuint shine_loc = glGetUniformLocation(this->shader_program, (prefix + "shine").c_str());
    glUniform1f(shine_loc, this->shine);
}

void material::set_material_fallback(const RC<material*>* obj_mat, bool has_diffuse, bool has_specular, bool has_normal, bool use_default_material_properties) {
    string prefix = "material.";
    // set struct parameters
    if (use_default_material_properties) {
        GLuint ambient_loc = glGetUniformLocation(this->shader_program, (prefix + "ambient").c_str());
        glUniform3fv(ambient_loc, 1, glm::value_ptr(this->ambient.axis));
    } else {
        GLuint ambient_loc = glGetUniformLocation(obj_mat->data->shader_program, (prefix + "ambient").c_str());
        glUniform3fv(ambient_loc, 1, glm::value_ptr(obj_mat->data->ambient.axis));
    }
    
    if (has_diffuse) {
        glActiveTexture(GL_TEX_N_ITTER[0]);
        obj_mat->data->diffuse_texture->data->bind();
    } else {
        glActiveTexture(GL_TEX_N_ITTER[0]);
        this->diffuse_texture->data->bind();
    }
    
    if (has_specular) {
        glActiveTexture(GL_TEX_N_ITTER[1]);
        obj_mat->data->specular_texture->data->bind();
    } else if (this->specular_texture != nullptr) {
        glActiveTexture(GL_TEX_N_ITTER[1]);
        this->specular_texture->data->bind();
    }
    
    if (use_default_material_properties) {
        GLuint shine_loc = glGetUniformLocation(this->shader_program, (prefix + "shine").c_str());
        glUniform1f(shine_loc, this->shine);
    } else {
        GLuint shine_loc = glGetUniformLocation(obj_mat->data->shader_program, (prefix + "shine").c_str());
        glUniform1f(shine_loc, obj_mat->data->shine);
    }

}