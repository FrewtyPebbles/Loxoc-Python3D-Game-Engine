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
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, this->position->axis);
    model *= glm::toMat4(glm::inverse(this->rotation->quat));
    model = glm::scale(model, this->scale->axis);

    glUseProgram(this->mat->shader_program);

    int model_loc = glGetUniformLocation(this->mat->shader_program, "model");
    int view_loc = glGetUniformLocation(this->mat->shader_program, "view");
    int projection_loc = glGetUniformLocation(this->mat->shader_program, "projection");

    glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(camera.view));
    glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(camera.projection));

    this->mat->register_uniforms();
    this->register_uniforms(); // register object level uniforms

    for (auto [_mesh_name, _mesh] : *this->mesh_data) {

        for(size_t t_n = 0; t_n < _mesh->diffuse_textures.size(); t_n++) {
            glActiveTexture(GL_TEX_N_ITTER[0]);
            _mesh->diffuse_textures[t_n]->bind();
        }

        // for(size_t t_n = 0; t_n < _mesh->specular_textures.size(); t_n++) {
        //     glActiveTexture(GL_TEX_N_ITTER[1]);
        //     _mesh->specular_textures[t_n]->bind();
        // }

        // for(size_t t_n = 0; t_n < _mesh->diffuse_textures.size(); t_n++) {
        //     glActiveTexture(GL_TEX_N_ITTER[2]);
        //     _mesh->specular_textures[t_n]->bind();
        // }

        glBindVertexArray(_mesh->gl_VAO);

        glDrawElements(GL_TRIANGLES, _mesh->indicies_size, GL_UNSIGNED_INT, 0);
        
        glBindVertexArray(0);
    }
}

void object::set_uniform(string name, uniform_type value, string type) {
    int loc = glGetUniformLocation(this->mat->shader_program, name.c_str());
    this->inner_set_uniform(loc, name, value, type);
}