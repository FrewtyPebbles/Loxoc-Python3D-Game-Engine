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

struct pixel {
    pixel(): x(0), y(0), color(make_tup<uint8_t, 3>({0,0,0})){}
    pixel(int x, int y, tup3ui8 color)
        : x(x), y(y), color(color) {}
    pixel(int x, int y)// this is used for hashing
        : x(x), y(y) {}
    pixel(const pixel& o) : x(o.x), y(o.y), color(o.color) {}
    int x;
    int y;
    tup3ui8 color;
    inline friend std::ostream& operator<<(std::ostream& os, const pixel& self) {
        os << "pixel<" << self.x << ", " << self.y << ", " << self.color << ">";
        return os;
    }
};

inline bool operator== (pixel const& lhs, pixel const& rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y; 
}

struct pixelHash {
    pixelHash()=default;
    size_t operator()(const pixel &pxl) const {
        return pxl.x + pxl.y + pxl.x > pxl.y;
    }
};

class camera {
public:
    camera();
    camera(vec3* position, int view_width, int view_height, float focal_length, float fov);
    ~camera();
    void render(vector<object*> objects);
    vec3* position;
    int view_width, view_height;
    float focal_length;
    float fov;
}; 
