#pragma once
#include "glad/gl.h"
#include "Object3d.h"
#include "Vec3.h"
#include "Quaternion.h"
#include "Texture.h"

class spot_light {
public:
    spot_light(){}
    spot_light(vec3* position, quaternion*  rotation, vec3* color, float cutOff, float outerCutOff, float intensity, float reach, bool use_cookie, rc_texture cookie = nullptr):
        position(position),
        rotation(rotation),
        color(color),
        cutOff(cutOff),
        outerCutOff(outerCutOff),
        intensity(intensity),
        reach(reach),
        use_cookie(use_cookie),
        cookie(cookie) {}
    vec3* position;
    quaternion* rotation;
    vec3* color;
    float cutOff;
    float outerCutOff;
    float intensity = 1.0f;
    float reach;
    float constant = 1.0f;
    float linear = 0.09f;
    float quadratic = 0.032f;
    bool use_cookie;
    rc_texture cookie = nullptr;
    void set_uniforms(GLuint shader_prog, size_t index);
    friend inline std::ostream& operator<<(std::ostream& os, const spot_light& self){
        os << "spot_light{ position: " << *self.position << ", direction: " << *self.rotation << ", color: " << *self.color << " }";
        return os;
    }
};