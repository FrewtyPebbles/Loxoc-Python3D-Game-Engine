#pragma once
#include "Vec3.h"
#include "Quaternion.h"
#include <vector>
#include "Material.h"
using std::vector;
class mesh;
class camera;
class mesh_dict;

// Shaders


class object {
public:
    object(){};
    object(mesh_dict* mesh_data, vec3* position, quaternion* rotation, vec3* scale, material* mat) : mesh_data(mesh_data), position(position), rotation(rotation), scale(scale), mat(mat) {}
    
    ~object(){}

    mesh_dict* mesh_data;
    vec3* position;
    quaternion* rotation;
    vec3* scale;
    material* mat;
    map<int, uniform_type> uniforms;
    void set_uniform(string name, uniform_type value, string type);
    void register_uniforms();

    void render(camera& camera);
};

typedef object object3d;