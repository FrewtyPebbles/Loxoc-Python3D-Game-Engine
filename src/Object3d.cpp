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
#include "Model.h"
#include "Animation.h"

#define ATTENUATION_THRESHOLD 0.003

object3d::object3d(rc_model model_data, vec3* position, quaternion* rotation, vec3* scale, rc_material mat, RC<collider*>* collider) : model_data(model_data), position(position), rotation(rotation), scale(scale), mat(mat) {
    if (collider) {
        this->colliders.push_back(collider);
    }
}

std::ostream& operator<<(std::ostream& os, const object3d& self){
    os << "object3d< \"" << self.model_data->data->mesh_data->data->name << "\" { position: " << *self.position << "} >";
    return os;
}
  
void object3d::render(camera& camera, window* window) {
    // opengl renderer
    this->get_model_matrix();

    // render mesh tree
    this->model_data->data->render(this, camera, window);
}

void object3d::set_uniform(string name, uniform_type value) {
    int loc = glGetUniformLocation(this->mat->data->shader_program, name.c_str());
    this->inner_set_uniform(loc, value);
}
