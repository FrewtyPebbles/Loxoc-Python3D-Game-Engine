#include "Object3d.h"
#include "Camera.h"
#include "Mesh.h"
#include <chrono>
#include <thread>
#include "util.h"
#include "glad/gl.h"
#include <SDL2/SDL_opengl.h>
#include "Texture.h"
#include <glm/gtx/rotate_vector.hpp>
#include "Window.h"


void object3d::render(camera& camera, window* window) {
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

    // camera view pos
    int view_pos_loc = glGetUniformLocation(this->mat->shader_program, "viewPos");
    glUniform3fv(view_pos_loc, 1, glm::value_ptr(camera.position->axis));

    // ambient light
    int ambient_light_loc = glGetUniformLocation(this->mat->shader_program, "ambient_light");
    glUniform3fv(ambient_light_loc, 1, glm::value_ptr(window->ambient_light->axis));

    this->mat->register_uniforms();
    this->register_uniforms(); // register object level uniforms
    size_t i;
    for (auto [_mesh_name, _meshes] : *this->mesh_data) {
        for (auto _mesh : _meshes) {
            for(size_t t_n = 0; t_n < _mesh->data->diffuse_textures.size(); t_n++) {
                glActiveTexture(GL_TEX_N_ITTER[0]);
                _mesh->data->diffuse_textures[t_n]->data->bind();
            }

            i = 0;
            for (point_light* pl : window->render_list_point_lights) {
                if (pl->position->distance(*this->position) <= pl->radius + _mesh->data->radius * this->scale->get_magnitude()) {
                    pl->set_uniforms(this->mat->shader_program, i);
                    i++;
                }
            }
            
            int total_point_lights_loc = glGetUniformLocation(this->mat->shader_program, "total_point_lights");
            glUniform1i(total_point_lights_loc, i);

            glBindVertexArray(_mesh->data->gl_VAO);

            glDrawElements(GL_TRIANGLES, _mesh->data->indicies_size, GL_UNSIGNED_INT, 0);
            
            glBindVertexArray(0);
        }
    }
}

void object3d::set_uniform(string name, uniform_type value, string type) {
    int loc = glGetUniformLocation(this->mat->shader_program, name.c_str());
    this->inner_set_uniform(loc, name, value, type);
}