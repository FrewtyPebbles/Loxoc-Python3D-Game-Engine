#include "Object.h"
#include "Camera.h"
#include "Mesh.h"
#include <chrono>
#include <thread>
#include "util.h"
#include "glad/gl.h"
#include <SDL2/SDL_opengl.h>
#include "Texture.h"
#include <glm/gtx/rotate_vector.hpp>



void object::render(camera& camera) {
    // opengl renderer
    glm::mat4 projection = glm::perspective(camera.fov, static_cast<float>(camera.view_width)/static_cast<float>(camera.view_height), 0.1f, static_cast<float>(camera.focal_length));
    glm::mat4 view = glm::mat4(1.0f);
    view *= glm::lookAt(camera.position->axis, camera.position->axis + camera.rotation->get_forward().axis, camera.rotation->get_up().axis);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, this->position->axis);
    model = glm::rotate(model, this->rotation->axis.x, glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, this->rotation->axis.y, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, this->rotation->axis.z, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, this->scale->axis);


    int model_loc = glGetUniformLocation(this->mat->shader_program, "model");
    int view_loc = glGetUniformLocation(this->mat->shader_program, "view");
    int projection_loc = glGetUniformLocation(this->mat->shader_program, "projection");

    glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection));

    this->mat->register_uniforms();
    this->register_uniforms(); // register object level uniforms
    
    glUseProgram(this->mat->shader_program);

    for (auto _mesh : this->mesh_data) {
        for(size_t t_n = 0; t_n < _mesh->diffuse_textures.size(); t_n++) {
            glActiveTexture(GL_TEX_N_ITTER[t_n]);
            _mesh->diffuse_textures[t_n]->bind();
        }

        glBindVertexArray(_mesh->gl_VAO);

        glDrawElements(GL_TRIANGLES, _mesh->indicies_size, GL_UNSIGNED_INT, 0);
        
        glBindVertexArray(0);
    }
}

void object::set_uniform(string name, uniform_type value, string type) {
    int loc = glGetUniformLocation(this->mat->shader_program, name.c_str());
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

void object::register_uniforms() {
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