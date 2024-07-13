#pragma once
#include "glad/gl.h"
#include "Object3d.h"
#include "Quaternion.h"

class directional_light {
public:
    directional_light(){}
    directional_light(quaternion* rotation, vec3* color, vec3* ambient, vec3* diffuse, vec3* specular)
        : rotation(rotation), color(color), ambient(ambient), diffuse(diffuse), specular(specular) {}
    quaternion* rotation;
    vec3* color;
    vec3* ambient;
    vec3* diffuse;
    vec3* specular;
    void set_uniforms(GLuint shader_prog, size_t index);
    friend inline std::ostream& operator<<(std::ostream& os, const directional_light& self){
        os << "directional_light{ rotation: " << *self.rotation << ", color: " << *self.color << " }";
        return os;
    }
};