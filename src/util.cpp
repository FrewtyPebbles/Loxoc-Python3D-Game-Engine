#include "util.h"
#include "Vec3.h"
#include "Vec2.h"

void c_set_mod_path(std::string path) {
  MOD_PATH = path;
}

std::string get_mod_path() {
  return MOD_PATH;
}

vec3 barycentric_coords(const vec2 & v1, const vec2 & v2, const vec2 & v3, const vec2 & p) {
    vec2 exp = p - v3;
    auto x1 = v1.axis.x, y1 = v1.axis.y;
    auto x2 = v2.axis.x, y2 = v2.axis.y;
    auto x3 = v3.axis.x, y3 = v3.axis.y;
    float det = fmaf((x1 - x3), (y2 - y3), -(x2 - x3)*(y1 - y3));
    float u1 = fmaf((y2 - y3), exp.axis.x, (x3 - x2)*exp.axis.y)/det;
    float u2 = fmaf((y3 - y1), exp.axis.x, (x1 - x3)*exp.axis.y)/det;
    float u3 = 1 - u1 - u2;
    return vec3(u1, u2, u3);
}

vec3 barycentric_coords(const vec3 & v1, const vec3 & v2, const vec3 & v3, const vec3 & p) {
    // edges and relative p vector
    vec3 e1 = v2 - v1;
    vec3 e2 = v3 - v1;
    vec3 prel = p - v1;

    float dot_e1_e1 = e1.dot(e1);
    float dot_e1_e2 = e1.dot(e2);
    float dot_e2_e2 = e2.dot(e2);
    float dot_prel_e1 = prel.dot(e1);
    float dot_prel_e2 = prel.dot(e2);

    float denom = (dot_e1_e1 * dot_e2_e2)-(dot_e1_e2 * dot_e1_e2);

    if (fabs(denom) < 1e-8f) {  // Adjusted tolerance for higher precision
        return vec3(-1.0f, -1.0f, -1.0f);  // Indicate failure
    }

    float u1 = (dot_e2_e2 * dot_prel_e1 - dot_e1_e2 * dot_prel_e2) / denom;
    float u2 = (dot_e1_e1 * dot_prel_e2 - dot_e1_e2 * dot_prel_e1) / denom;
    float u3 = 1.0f - u1 - u2;

    if (u1 < -1e-6f || u2 < -1e-6f || u3 < -1e-6f) {
        return vec3(-1.0f, -1.0f, -1.0f);  // Return invalid result if any coordinate is out of range
    }

    return vec3(u1, u2, u3);
}