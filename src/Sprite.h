#pragma once
#include "Vec2.h"
#include "Texture.h"
#include <vector>

using std::vector;
#define norm(a) a.get_normalized()

class sprite {
public:
    sprite(){};
    sprite(texture* tex)
    :
        tex(tex),
        quad{
            norm(vec2(-tex->width/2, tex->height/2)), norm(vec2(tex->width/2, tex->height/2)),
            norm(vec2(-tex->width/2, -tex->height/2)), norm(vec2(tex->width/2, -tex->height/2))
        }
    { create_VAO(); }
    

    texture* tex;
    vec2 quad[4];
    GLuint gl_VAO, gl_VBO, gl_EBO;
private:
    void create_VAO();
};