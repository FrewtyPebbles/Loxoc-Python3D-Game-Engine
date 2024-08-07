#pragma once
#include <vector>
#include "Tup.h"
#include <unordered_set>
#include "glad/gl.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include "Quaternion.h"
#include "Vec3.h"
#include "Matrix.h"

using std::vector;
using std::unordered_set;
class object3d;

class camera {
public:
    camera();
    camera(vec3* position, quaternion* rotation, int view_width, int view_height, float focal_length, float fov);
    ~camera();
    vec3* position;
    quaternion* rotation;
    int view_width, view_height;
    float focal_length;
    float fov;
    void recalculate_pv();
    matrix4x4 projection, view;
}; 
