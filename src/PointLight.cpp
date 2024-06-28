#include "PointLight.h"
#include <sstream>

void point_light::set_uniforms(GLuint shader_prog, size_t index) {
    std::stringstream ss;
    ss << "point_lights[" << index << "].";
    auto prefix = ss.str();
    // set struct parameters
    GLuint position_loc = glGetUniformLocation(shader_prog, (prefix + "position").c_str());
    glUniform3fv(position_loc, 1, glm::value_ptr(this->position->axis));

    GLuint color_loc = glGetUniformLocation(shader_prog, (prefix + "color").c_str());
    glUniform3fv(color_loc, 1, glm::value_ptr(this->color->axis));

    GLuint radius_loc = glGetUniformLocation(shader_prog, (prefix + "radius").c_str());
    glUniform1f(radius_loc, this->radius);
}