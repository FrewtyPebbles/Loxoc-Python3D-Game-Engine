#pragma once
#include "Vec2.h"
#include <vector>
#include "Material.h"
#include "Sprite.h"

using std::vector;
class camera;


class object2d : public TRAIT_has_uniform {
public:
    object2d(){};
    object2d(sprite* spr, vec2* position, float rotation, vec2* scale, rc_material mat, float depth)
    :
        spr(spr),
        position(position),
        depth(depth),
        rotation(rotation),
        scale(scale),
        mat(mat),
        spr_width(spr->width),
        spr_height(spr->height)
    {}
    
    sprite* spr = nullptr;
    vec2* position = nullptr;
    float depth = 0.0f;
    float rotation = 0.0f;
    vec2* scale = nullptr;
    rc_material mat;
    int * spr_width;
    int * spr_height;
    void set_uniform(string name, uniform_type value);
    void render(camera& camera);
};

