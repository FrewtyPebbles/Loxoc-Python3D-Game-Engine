#pragma once
#include "Vec3.h"
#include <vector>
#include "Material.h"
using std::vector;
class mesh;
class camera;

// Shaders


class object {
public:
    object(){};
    object(mesh* mesh, vec3 position, vec3 rotation, vec3 scale, material* mat = nullptr) : mesh_data(mesh), position(position), rotation(rotation), scale(scale) {
        this->mat = (!mat) ? new material() : mat;
    }
    
    ~object(){
        delete mat;
    }

    mesh* mesh_data;
    vec3 position;
    vec3 rotation;
    vec3 scale;
    material* mat;

    void render(camera& camera);
};

typedef object object3d;