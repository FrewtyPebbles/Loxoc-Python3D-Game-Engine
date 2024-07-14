#include "DirectionalLight.h"
#include <sstream>

void directional_light::set_uniforms(GLuint shader_prog, size_t index) {
    std::stringstream ss;
    ss << "directional_lights[" << index << "].";
    auto prefix = ss.str();
    // set struct parameters
    GLuint rotation_loc = glGetUniformLocation(shader_prog, (prefix + "rotation").c_str());
    glUniformMatrix4fv(rotation_loc, 1, GL_FALSE, glm::value_ptr(glm::toMat4(this->rotation->quat)));

    GLuint color_loc = glGetUniformLocation(shader_prog, (prefix + "color").c_str());
    glUniform3fv(color_loc, 1, glm::value_ptr(this->color->axis));

    GLuint ambient_loc = glGetUniformLocation(shader_prog, (prefix + "ambient").c_str());
    glUniform3fv(ambient_loc, 1, glm::value_ptr(this->ambient->axis));

    GLuint diffuse_loc = glGetUniformLocation(shader_prog, (prefix + "diffuse").c_str());
    glUniform3fv(diffuse_loc, 1, glm::value_ptr(this->diffuse->axis));

    GLuint specular_loc = glGetUniformLocation(shader_prog, (prefix + "specular").c_str());
    glUniform3fv(specular_loc, 1, glm::value_ptr(this->specular->axis));

    GLuint intensity_loc = glGetUniformLocation(shader_prog, (prefix + "intensity").c_str());
    glUniform1f(intensity_loc, this->intensity);
}