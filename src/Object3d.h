#pragma once
#include "Vec3.h"
#include "Quaternion.h"
#include <vector>
#include "Material.h"
#include "RC.h"
#include <iostream>
#include "Octree.h"
#include "Colliders.h"

using std::vector;
class mesh;
class camera;
class mesh_dict;
class window;
// Shaders


class object3d : public TRAIT_has_uniform {
public:
    object3d(){};
    object3d(RC<mesh_dict*>* mesh_data, vec3* position, quaternion* rotation, vec3* scale, rc_material mat = nullptr, RC<collider*>* collider = nullptr) : mesh_data(mesh_data), position(position), rotation(rotation), scale(scale), mat(mat) {
        this->mesh_data = mesh_data;
        if (collider)
            this->colliders.push_back(collider);
    }
    
    ~object3d(){}

    RC<mesh_dict*>* mesh_data;
    vec3* position;
    quaternion* rotation;
    vec3* scale;
    rc_material mat;
    map<int, uniform_type> uniforms;
    vector<RC<collider*>*> colliders;
    octree<RC<collider*>*>* all_colliders;
    glm::mat4 model_matrix = get_model_matrix();

    void set_uniform(string name, uniform_type value, string type);

    void render(camera& camera, window* window);

    friend std::ostream& operator<<(std::ostream& os, const object3d& self);

    inline glm::mat4 get_model_matrix() {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, this->position->axis);
        model *= glm::toMat4(this->rotation->quat);
        model = glm::scale(model, this->scale->axis);
        this->model_matrix = model;
        return model;
    }



    inline bool check_collision_point(vec3 point) {
        for (auto collider : this->colliders) {
            if (collider->data->check_collision(point))
                return true;
        }
        return false;
    }

    inline bool check_collision_object(object3d* obj) {
        for (auto collider : this->colliders) {
            for (auto other_collider : obj->colliders) {
                if (collider->data->check_collision(other_collider->data))
                    return true;
            }
        }
        return false;
    }
private:
    void render_meshdict(RC<mesh_dict*>* _mesh_data, camera& camera, window* window);
};