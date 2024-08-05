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
#include "SpotLight.h"


#define ATTENUATION_THRESHOLD 0.003

std::ostream& operator<<(std::ostream& os, const object3d& self){
    os << "object3d< \"" << self.mesh_data->data->name << "\" { position: " << *self.position << "} >";
    return os;
}

void object3d::render(camera& camera, window* window) {
    // opengl renderer
    this->get_model_matrix();
    if (this->mat != nullptr) {
        glUseProgram(this->mat->data->shader_program);

        int model_loc = glGetUniformLocation(this->mat->data->shader_program, "model");
        int view_loc = glGetUniformLocation(this->mat->data->shader_program, "view");
        int projection_loc = glGetUniformLocation(this->mat->data->shader_program, "projection");

        glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(this->model_matrix.mat));
        glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(camera.view));
        glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(camera.projection));

        // camera view pos
        int view_pos_loc = glGetUniformLocation(this->mat->data->shader_program, "viewPos");
        glUniform3fv(view_pos_loc, 1, glm::value_ptr(camera.position->axis));

        // ambient light
        int ambient_light_loc = glGetUniformLocation(this->mat->data->shader_program, "ambient_light");
        glUniform3fv(ambient_light_loc, 1, glm::value_ptr(window->ambient_light->axis));

        this->mat->data->set_material();

        this->mat->data->register_uniforms();
        this->register_uniforms(); // register object level uniforms
    }

    // render mesh tree
    this->render_meshdict(this->mesh_data, camera, window);
}

void object3d::render_meshdict(RC<mesh_dict*>* _mesh_data, camera& camera, window* window) {
    size_t i;
    for (auto [_mesh_name, _mesh_variant] : *_mesh_data->data) {
        if (std::holds_alternative<rc_mesh>(_mesh_variant)) {
            auto _mesh = std::get<rc_mesh>(_mesh_variant);
            // get material from mesh if object does not have a material
            if (this->mat != nullptr) if (this->mat->data->diffuse_texture != nullptr) {
                glActiveTexture(GL_TEX_N_ITTER[0]);
                this->mat->data->diffuse_texture->data->bind();
            }

            if (this->mat == nullptr) { // Use mesh material.

                glUseProgram(_mesh->data->mesh_material->data->shader_program);

                int model_loc = glGetUniformLocation(_mesh->data->mesh_material->data->shader_program, "model");
                int view_loc = glGetUniformLocation(_mesh->data->mesh_material->data->shader_program, "view");
                int projection_loc = glGetUniformLocation(_mesh->data->mesh_material->data->shader_program, "projection");

                glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(this->model_matrix.mat));
                glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(camera.view));
                glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(camera.projection));

                // camera view pos
                int view_pos_loc = glGetUniformLocation(_mesh->data->mesh_material->data->shader_program, "viewPos");
                glUniform3fv(view_pos_loc, 1, glm::value_ptr(camera.position->axis));

                // ambient light
                int ambient_light_loc = glGetUniformLocation(_mesh->data->mesh_material->data->shader_program, "ambient_light");
                glUniform3fv(ambient_light_loc, 1, glm::value_ptr(window->ambient_light->axis));

                _mesh->data->mesh_material->data->set_material();

                _mesh->data->mesh_material->data->register_uniforms();
                this->register_uniforms(); // register object level uniforms
                
                // Point Lights:
                
                i = 0;
                for (point_light* pl : window->render_list_point_lights) {
                    // calculate when to remove light by having an attenuation threshhold.
                    float l_distance = pl->position->distance(*this->position);
                    float attenuation = 1.0 / (pl->constant + pl->linear * l_distance + (1/(pl->radius*pl->radius)) * (l_distance * l_distance));
                    if (attenuation > ATTENUATION_THRESHOLD) {
                        pl->set_uniforms(_mesh->data->mesh_material->data->shader_program, i);
                        i++;
                    }
                }


                int total_point_lights_loc = glGetUniformLocation(_mesh->data->mesh_material->data->shader_program, "total_point_lights");

                glUniform1i(total_point_lights_loc, i);
                
                // Directional Lights:
                
                i = 0;
                for (directional_light* dl : window->render_list_directional_lights) {
                    dl->set_uniforms(_mesh->data->mesh_material->data->shader_program, i);
                    i++;
                }

                int total_directional_lights_loc = glGetUniformLocation(_mesh->data->mesh_material->data->shader_program, "total_directional_lights");

                glUniform1i(total_directional_lights_loc, i);

                // Spot Lights:

                i = 0;
                for (spot_light* sl : window->render_list_spot_lights) {
                    // calculate when to remove light by having an attenuation threshhold.
                    float l_distance = sl->position->distance(*this->position);
                    float attenuation = 1.0 / (sl->constant + sl->linear * l_distance + (1/(sl->reach*sl->reach)) * (l_distance * l_distance));
                    if (attenuation > ATTENUATION_THRESHOLD) {
                        sl->set_uniforms(_mesh->data->mesh_material->data->shader_program, i);
                        i++;
                    }
                }
                
                int total_spot_lights_loc = glGetUniformLocation(_mesh->data->mesh_material->data->shader_program, "total_spot_lights");

                glUniform1i(total_spot_lights_loc, i);


            } else { // Use object material.

                // Point Lights:

                i = 0;
                for (point_light* pl : window->render_list_point_lights) {
                    // calculate when to remove light by having an attenuation threshhold.
                    float l_distance = pl->position->distance(*this->position);
                    float attenuation = 1.0 / (pl->constant + pl->linear * l_distance + (1/(pl->radius*pl->radius)) * (l_distance * l_distance));
                    if (attenuation > ATTENUATION_THRESHOLD) {
                        pl->set_uniforms(this->mat->data->shader_program, i);
                        i++;
                    }
                }
                
                int total_point_lights_loc = glGetUniformLocation(this->mat->data->shader_program, "total_point_lights");

                glUniform1i(total_point_lights_loc, i);
                

                // Directional Lights:
                
                i = 0;
                for (directional_light* dl : window->render_list_directional_lights) {
                    dl->set_uniforms(this->mat->data->shader_program, i);
                    i++;
                }

                int total_directional_lights_loc = glGetUniformLocation(this->mat->data->shader_program, "total_directional_lights");

                glUniform1i(total_directional_lights_loc, i);

                // Spot Lights:

                i = 0;
                for (spot_light* sl : window->render_list_spot_lights) {
                    // calculate when to remove light by having an attenuation threshhold.
                    float l_distance = sl->position->distance(*this->position);
                    float attenuation = 1.0 / (sl->constant + sl->linear * l_distance + (1/(sl->reach*sl->reach)) * (l_distance * l_distance));
                    if (attenuation > ATTENUATION_THRESHOLD) {
                        sl->set_uniforms(this->mat->data->shader_program, i);
                        i++;
                    }
                }
                
                int total_spot_lights_loc = glGetUniformLocation(this->mat->data->shader_program, "total_spot_lights");

                glUniform1i(total_spot_lights_loc, i);
            }
            
            glBindVertexArray(_mesh->data->gl_VAO);
            
            glDrawElements(GL_TRIANGLES, _mesh->data->indicies_size, GL_UNSIGNED_INT, 0);
            
            glBindVertexArray(0);
        } else if (std::holds_alternative<rc_mesh_dict>(_mesh_variant)) {
            auto _mesh_dict = std::get<rc_mesh_dict>(_mesh_variant);
            this->render_meshdict(_mesh_dict, camera, window);
        }
    }
}

void object3d::set_uniform(string name, uniform_type value) {
    int loc = glGetUniformLocation(this->mat->data->shader_program, name.c_str());
    this->inner_set_uniform(loc, value);
}
