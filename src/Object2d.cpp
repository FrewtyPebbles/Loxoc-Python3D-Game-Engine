#include "Object2d.h"
#include "glad/gl.h"
#include <SDL2/SDL_opengl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtc/type_ptr.hpp>

void object2d::set_uniform(string name, uniform_type value, string type) {
    int loc = glGetUniformLocation(this->mat->shader_program, name.c_str());
    this->inner_set_uniform(loc, name, value, type);
}

void object2d::render(camera& camera) {
    // opengl renderer
    glm::mat4 transform2D = glm::mat4(1.0f);
    glUseProgram(this->mat->shader_program);
    transform2D = glm::translate(transform2D, glm::vec3(this->position->axis, 0.0f));
    transform2D = glm::rotate(transform2D, this->rotation, glm::vec3(0.0f, 0.0f, 1.0f));
    transform2D = glm::scale(transform2D, glm::vec3(this->scale->axis, 1.0f));

    int transform2D_loc = glGetUniformLocation(this->mat->shader_program, "transform2D");

    glUniformMatrix4fv(transform2D_loc, 1, GL_FALSE, glm::value_ptr(transform2D));

    this->mat->register_uniforms();
    this->register_uniforms(); // register object level uniforms

    this->spr->tex->data->bind();

    glBindVertexArray(this->spr->gl_VAO);

    glDrawElements(GL_TRIANGLES, 6 * sizeof(GLuint), GL_UNSIGNED_INT, 0);
    
    glBindVertexArray(0);

    
}