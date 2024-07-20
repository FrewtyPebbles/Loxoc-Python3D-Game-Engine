#include "SpotLight.h"
#include <sstream>

void spot_light::set_uniforms(GLuint shader_prog, size_t index) {
    std::stringstream ss;
    ss << "spot_lights[" << index << "].";
    auto prefix = ss.str();
    // set struct parameters

    // if (this->use_cookie) {
    //     glActiveTexture(GL_TEX_N_ITTER[2]);
    //     this->cookie->data->bind();
    // }

    GLuint position_loc = glGetUniformLocation(shader_prog, (prefix + "position").c_str());
    glUniform3fv(position_loc, 1, glm::value_ptr(this->position->axis));

    GLuint rotation_loc = glGetUniformLocation(shader_prog, (prefix + "rotation").c_str());
    glUniformMatrix4fv(rotation_loc, 1, GL_FALSE, glm::value_ptr(glm::toMat4(this->rotation->quat)));

    GLuint color_loc = glGetUniformLocation(shader_prog, (prefix + "color").c_str());
    glUniform3fv(color_loc, 1, glm::value_ptr(this->color->axis));

    GLuint cutOff_loc = glGetUniformLocation(shader_prog, (prefix + "cutOff").c_str());
    glUniform1f(cutOff_loc, glm::cos(this->cutOff));

    GLuint outerCutOff_loc = glGetUniformLocation(shader_prog, (prefix + "outerCutOff").c_str());
    glUniform1f(outerCutOff_loc, glm::cos(this->outerCutOff));

    // GLuint use_cookie_loc = glGetUniformLocation(shader_prog, (prefix + "use_cookie").c_str());
    // glUniform1i(use_cookie_loc, this->use_cookie);

    GLuint constant_loc = glGetUniformLocation(shader_prog, (prefix + "constant").c_str());
    glUniform1f(constant_loc, this->constant);

    GLuint linear_loc = glGetUniformLocation(shader_prog, (prefix + "linear").c_str());
    glUniform1f(linear_loc, this->linear);

    GLuint quadratic_loc = glGetUniformLocation(shader_prog, (prefix + "quadratic").c_str());
    glUniform1f(quadratic_loc, 1/(this->reach*this->reach));

    GLuint intensity_loc = glGetUniformLocation(shader_prog, (prefix + "intensity").c_str());
    glUniform1f(intensity_loc, this->intensity);
} 