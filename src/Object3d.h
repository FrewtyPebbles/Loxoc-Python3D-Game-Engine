#pragma once
#include "Vec3.h"
#include "Quaternion.h"
#include <vector>
#include "Material.h"
using std::vector;
class mesh;
class camera;
class mesh_dict;
class window;
// Shaders


class object3d : public TRAIT_has_uniform {
public:
    object3d(){};
    object3d(mesh_dict* mesh_data, vec3* position, quaternion* rotation, vec3* scale, material* mat) : mesh_data(mesh_data), position(position), rotation(rotation), scale(scale), mat(mat) {}
    
    ~object3d(){}

    mesh_dict* mesh_data;
    vec3* position;
    quaternion* rotation;
    vec3* scale;
    material* mat;
    map<int, uniform_type> uniforms;
    void set_uniform(string name, uniform_type value, string type);

    void render(camera& camera, window* window);
};