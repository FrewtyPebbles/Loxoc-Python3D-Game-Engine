#include "Object2d.h"
#include "glad/gl.h"
#include <SDL2/SDL_opengl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"
#include "Window.h"

void object2d::set_uniform(string name, uniform_type value) {
    int loc = glGetUniformLocation(this->mat->data->shader_program, name.c_str());
    this->inner_set_uniform(loc, value);
}

void object2d::render(camera& camera) {
    // opengl renderer
    matrix4x4 transform2D = matrix4x4(1.0f);
    this->mat->data->use_material(); // Use your shader program

    // Set uniforms like text color
    this->mat->data->set_uniform("projection", matrix4x4::from_ortho(0.0f, (float)camera.win->width, 0.0f, (float)camera.win->height, -999999.0f, 999999.0f));

    transform2D = transform2D.translate(vec3(*this->position, -this->depth));
    transform2D = transform2D.rotate(this->rotation, vec3(0.0f, 0.0f, 1.0f));
    transform2D = transform2D.scale(vec3(this->scale->axis, 1.0f));

    this->mat->data->set_uniform("transform2D", transform2D);

    this->mat->data->register_uniforms();
    this->register_uniforms(); // register object level uniforms

    this->spr->tex->data->bind();

    glBindVertexArray(this->spr->gl_VAO);

    glDrawElements(GL_TRIANGLES, 6 * sizeof(GLuint), GL_UNSIGNED_INT, 0);
    
    glBindVertexArray(0);

    
}
 