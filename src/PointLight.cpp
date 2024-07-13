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

    GLuint constant_loc = glGetUniformLocation(shader_prog, (prefix + "constant").c_str());
    glUniform1f(constant_loc, this->constant);

    GLuint linear_loc = glGetUniformLocation(shader_prog, (prefix + "linear").c_str());
    glUniform1f(linear_loc, this->linear);

    GLuint quadratic_loc = glGetUniformLocation(shader_prog, (prefix + "quadratic").c_str());
    glUniform1f(quadratic_loc, 1/(this->radius*this->radius));
}