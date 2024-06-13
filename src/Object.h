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
    object(vector<mesh*> mesh, vec3 position, vec3 rotation, vec3 scale, material* mat) : mesh_data(mesh), position(position), rotation(rotation), scale(scale), mat(mat) {}
    
    ~object(){}

    vector<mesh*> mesh_data;
    vec3 position;
    vec3 rotation;
    vec3 scale = vec3(1.0f, 1.0f, 1.0f);
    material* mat;
    map<int, uniform_type> uniforms;
    void set_uniform(string name, uniform_type value, string type);
    void register_uniforms();

    void render(camera& camera);
};

typedef object object3d;