#include "Object2d.h"
#include "glad/gl.h"
#include <SDL2/SDL_opengl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"
#include "Window.h"

object2d::object2d(sprite* spr, camera * cam, vec2* position, float rotation, vec2* scale, rc_material mat, float depth)
:
    spr(spr),
    cam(cam),
    position(position),
    depth(depth),
    rotation(rotation),
    scale(scale),
    mat(mat),
    spr_width(spr->width),
    spr_height(spr->height)
{
    auto ortho = matrix4x4::from_ortho(0.0f, (float)cam->view_width, 0.0f, (float)cam->view_height, -999999.0f, 999999.0f);
    auto w_low = ortho * vec4(spr->quad[0], this->depth, 1.0f);
    auto w_high = ortho * vec4(spr->quad[1], this->depth, 1.0f);
    auto h_low = ortho * vec4(spr->quad[2], this->depth, 1.0f);
    auto h_high = ortho * vec4(spr->quad[0], this->depth, 1.0f);
    unscaled_dim = vec2(w_low.distance(w_high), h_low.distance(h_high));
}

vec2 object2d::get_scaled_dimensions() {
    auto scale_m = matrix4x4(1.0).scale(vec3(*scale, 1.0f));
    auto ortho = matrix4x4::from_ortho(0.0f, (float)cam->view_width, 0.0f, (float)cam->view_height, -999999.0f, 999999.0f);
    auto q0 = ortho * scale_m * vec4(spr->quad[0], 0.0f, 1.0f);
    auto q1 = ortho * scale_m * vec4(spr->quad[1], 0.0f, 1.0f);
    auto q2 = ortho * scale_m * vec4(spr->quad[2], 0.0f, 1.0f);
    auto scaled_dimensions = vec2(q0.distance(q1) * cam->view_width / 2.0f, q2.distance(q0) * cam->view_height / 2.0f);
    return scaled_dimensions;
}

void object2d::set_uniform(string name, uniform_type value) {
    int loc = glGetUniformLocation(this->mat->data->shader_program, name.c_str());
    this->inner_set_uniform(loc, value);
}

void object2d::render(camera& camera) {
    this->cam = &camera;
    // opengl renderer
    matrix4x4 transform2D = matrix4x4(1.0f);
    this->mat->data->use_material(); // Use your shader program

    // Set uniforms like text color
    this->mat->data->set_uniform("projection", matrix4x4::from_ortho(0.0f, (float)camera.view_width, 0.0f, (float)camera.view_height, -999999.0f, 999999.0f));

    transform2D = transform2D.translate(vec3(*this->position,0.0f));
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