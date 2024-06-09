#pragma once
#include "Vec3.h"
#include <vector>
using std::vector;
class mesh;
class camera;

// Shaders


class object {
public:
    object(){};
    // create 3 different constructors for the default arguments in .pxd file
    object(mesh* mesh, vec3 position, vec3 rotation, vec3 scale);
    
    ~object(){}

    mesh* mesh_data;
    vec3 position;
    vec3 rotation;
    vec3 scale;

    void render(camera& camera);
    vector<vec3> get_translation(vector<vec3> vertexes);
    vector<vec3> get_rotation(vector<vec3> vertexes);
};

typedef object object3d;