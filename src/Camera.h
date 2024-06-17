#pragma once
#include <vector>
#include "Tup.h"
#include <unordered_set>
#include <windows.h>
#include "glad/gl.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

using std::vector;
using std::unordered_set;
class vec3;
class object;

class camera {
public:
    camera();
    camera(vec3* position, vec3* rotation, int view_width, int view_height, float focal_length, float fov);
    ~camera();
    void render(vector<object*> objects);
    vec3* position;
    vec3* rotation;
    int view_width, view_height;
    float focal_length;
    float fov;
}; 
