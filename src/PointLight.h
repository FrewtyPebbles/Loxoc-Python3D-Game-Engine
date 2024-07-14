#pragma once
#include "glad/gl.h"
#include "Object3d.h"
#include "Vec3.h"

class point_light {
public:
    point_light(){}
    point_light(vec3* position, float radius, vec3* color, float intensity)
        : position(position), radius(radius), color(color), intensity(intensity) {}
    vec3* position;
    float radius;
    vec3* color;
    float intensity = 1.0f;
    float constant = 1.0f;
    float linear = 0.09f;
    float quadratic = 0.032f;
    void set_uniforms(GLuint shader_prog, size_t index);
    friend inline std::ostream& operator<<(std::ostream& os, const point_light& self){
        os << "point_light{ position: " << *self.position << ", radius: " << self.radius << ", color: " << *self.color << " }";
        return os;
    }
};