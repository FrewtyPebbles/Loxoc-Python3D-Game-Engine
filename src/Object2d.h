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
    object2d(sprite* spr, vec2* position, float rotation, vec2* scale, rc_material mat)
    :
        spr(spr),
        position(position),
        rotation(rotation),
        scale(scale),
        mat(mat)
    {}
    
    sprite* spr;
    vec2* position;
    float rotation;
    vec2* scale;
    rc_material mat;
    void set_uniform(string name, uniform_type value);
    void render(camera& camera);
};

