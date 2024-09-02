#include "Model.h"
#include "Animation.h"

#define ATTENUATION_THRESHOLD 0.003

void model::play_animation(const string& animation) {
    animation_player->play(animations[animation]);
}

model::~model() {
    delete animation_player;
    for (auto & [k, v] : animations)
        delete v;
}

model::model(RC<mesh_dict*>* mesh_data, bool animated) : mesh_data(mesh_data), animated(animated), animation_player(new animator(nullptr)) {}

void model::render_meshdict(RC<mesh_dict*>* _mesh_data, object3d* obj, camera& camera, window* window) {
    for (auto [_mesh_name, _mesh_variant] : *_mesh_data->data) {
        if (std::holds_alternative<rc_mesh>(_mesh_variant)) {
            auto _mesh = std::get<rc_mesh>(_mesh_variant);
            // get material from mesh if object does not have a material
            if (obj->mat != nullptr) if (obj->mat->data->diffuse_texture != nullptr) {
                glActiveTexture(GL_TEX_N_ITTER[0]);
                obj->mat->data->diffuse_texture->data->bind();
            }

            if (obj->mat == nullptr) { // Use mesh material.

                // set mvp 
                _mesh->data->mesh_material->data->use_material();
                _mesh->data->mesh_material->data->set_uniform("model", obj->model_matrix);
                _mesh->data->mesh_material->data->set_uniform("view", camera.view);
                _mesh->data->mesh_material->data->set_uniform("projection", camera.projection);

                // camera view pos
                _mesh->data->mesh_material->data->set_uniform("viewPos", *camera.position);

                // ambient light
                _mesh->data->mesh_material->data->set_uniform("ambient_light", *window->ambient_light);

                _mesh->data->mesh_material->data->set_material();

                
                
                // Point Lights:
                
                size_t i = 0;
                for (point_light* pl : window->render_list_point_lights) {
                    // calculate when to remove light by having an attenuation threshhold.
                    float l_distance = pl->position->distance(*obj->position);
                    float attenuation = 1.0 / (pl->constant + pl->linear * l_distance + (1/(pl->radius*pl->radius)) * (l_distance * l_distance));
                    if (attenuation > ATTENUATION_THRESHOLD) {
                        pl->set_uniforms(_mesh->data->mesh_material->data->shader_program, i);
                        i++;
                    }
                }

                _mesh->data->mesh_material->data->set_uniform("total_point_lights", static_cast<int>(i));
                
                // Directional Lights:
                
                i = 0;
                for (directional_light* dl : window->render_list_directional_lights) {
                    dl->set_uniforms(_mesh->data->mesh_material->data->shader_program, i);
                    i++;
                }

                _mesh->data->mesh_material->data->set_uniform("total_directional_lights", static_cast<int>(i));

                // Spot Lights:

                i = 0;
                for (spot_light* sl : window->render_list_spot_lights) {
                    // calculate when to remove light by having an attenuation threshhold.
                    float l_distance = sl->position->distance(*obj->position);
                    float attenuation = 1.0 / (sl->constant + sl->linear * l_distance + (1/(sl->reach*sl->reach)) * (l_distance * l_distance));
                    if (attenuation > ATTENUATION_THRESHOLD) {
                        sl->set_uniforms(_mesh->data->mesh_material->data->shader_program, i);
                        i++;
                    }
                }

                _mesh->data->mesh_material->data->set_uniform("total_spot_lights", static_cast<int>(i));

                // update animations
                if (obj->model_data->data->animated)
                    obj->model_data->data->animation_player->set_uniforms(_mesh->data->mesh_material);
  
                _mesh->data->mesh_material->data->register_uniforms();
                obj->register_uniforms(); // register object level uniforms

            } else { // Use object material.

                _mesh->data->mesh_material->data->set_material_fallback(
                    obj->mat->data->diffuse_texture == nullptr,
                    obj->mat->data->specular_texture == nullptr,
                    obj->mat->data->normals_texture == nullptr,
                    use_default_material_properties || (obj->mat == nullptr)
                );
                // Point Lights:

                size_t i = 0;
                for (point_light* pl : window->render_list_point_lights) {
                    // calculate when to remove light by having an attenuation threshhold.
                    float l_distance = pl->position->distance(*obj->position);
                    float attenuation = 1.0 / (pl->constant + pl->linear * l_distance + (1/(pl->radius*pl->radius)) * (l_distance * l_distance));
                    if (attenuation > ATTENUATION_THRESHOLD) {
                        pl->set_uniforms(obj->mat->data->shader_program, i);
                        i++;
                    }
                }
                
                obj->mat->data->set_uniform("total_point_lights", static_cast<int>(i));

                // Directional Lights:
                
                i = 0;
                for (directional_light* dl : window->render_list_directional_lights) {
                    dl->set_uniforms(obj->mat->data->shader_program, i);
                    i++;
                }

                obj->mat->data->set_uniform("total_directional_lights", static_cast<int>(i));

                // Spot Lights:

                i = 0; 
                for (spot_light* sl : window->render_list_spot_lights) {
                    // calculate when to remove light by having an attenuation threshhold.
                    float l_distance = sl->position->distance(*obj->position);
                    float attenuation = 1.0 / (sl->constant + sl->linear * l_distance + (1/(sl->reach*sl->reach)) * (l_distance * l_distance));
                    if (attenuation > ATTENUATION_THRESHOLD) {
                        sl->set_uniforms(obj->mat->data->shader_program, i);
                        i++;
                    }
                }

                obj->mat->data->set_uniform("total_spot_lights", static_cast<int>(i));

                // update animations
                if (obj->model_data->data->animated)
                    obj->model_data->data->animation_player->set_uniforms(obj->mat);
                
                obj->mat->data->register_uniforms();
            }
            
            glBindVertexArray(_mesh->data->gl_VAO);
            
            glDrawElements(GL_TRIANGLES, _mesh->data->indicies_size, GL_UNSIGNED_INT, 0);
            
            glBindVertexArray(0);
        } else if (std::holds_alternative<rc_mesh_dict>(_mesh_variant)) {
            auto _mesh_dict = std::get<rc_mesh_dict>(_mesh_variant);
            this->render_meshdict(_mesh_dict, obj, camera, window);
        }
    }
}