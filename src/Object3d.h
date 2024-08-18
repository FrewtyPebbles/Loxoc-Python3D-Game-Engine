#pragma once
#include "Vec3.h"
#include "Quaternion.h"
#include <vector>
#include "Material.h"
#include "RC.h"
#include <iostream>
#include "Octree.h"
#include "Colliders.h"
#include "Matrix.h"


using std::vector;
class mesh;
class camera;
class mesh_dict;
class window;
class model;

typedef RC<model*>* rc_model;


class object3d : public TRAIT_has_uniform {
public:
    object3d(){};
    object3d(rc_model model_data, vec3* position, quaternion* rotation, vec3* scale, rc_material mat = nullptr, RC<collider*>* collider = nullptr);
    
    ~object3d(){}

    rc_model model_data;
    vec3* position;
    quaternion* rotation;
    vec3* scale;
    rc_material mat;
    map<int, uniform_type> uniforms;
    vector<RC<collider*>*> colliders;
    octree<RC<collider*>*>* all_colliders;
    matrix4x4 model_matrix = get_model_matrix();

    void set_uniform(string name, uniform_type value);

    void render(camera& camera, window* window);

    friend std::ostream& operator<<(std::ostream& os, const object3d& self);

    inline matrix4x4 get_model_matrix() {
        matrix4x4 model = matrix4x4(1.0f);
        model = model.translate(this->position);
        model *= matrix4x4(this->rotation);
        model = model.scale(this->scale);
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
};