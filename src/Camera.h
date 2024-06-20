#pragma once
#include <vector>
#include "Tup.h"
#include <unordered_set>
#include <windows.h>
#include "glad/gl.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include "Quaternion.h"
#include "Vec3.h"

using std::vector;
using std::unordered_set;
class object;

class camera {
public:
    camera();
    camera(vec3* position, quaternion* rotation, int view_width, int view_height, float focal_length, float fov);
    ~camera();
    void render(vector<object*> objects);
    vec3* position;
    quaternion* rotation;
    int view_width, view_height;
    float focal_length;
    float fov;
}; 
