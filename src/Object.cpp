#include "Object.h"
#include "Camera.h"
#include "Mesh.h"
#include <chrono>
#include <thread>
#include "util.h"
#include "glad/gl.h"
#include <SDL2/SDL_opengl.h>


object::object(mesh* mesh, vec3 position, vec3 rotation, vec3 scale)
: mesh_data(mesh), position(position), rotation(rotation), scale(scale) {}


void object::render(camera& camera) {
    // opengl renderer
    glm::mat4 projection = glm::perspective(camera.fov, static_cast<float>(camera.view_width)/static_cast<float>(camera.view_height), 0.1f, static_cast<float>(camera.focal_length));
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, this->position.axis);  // Move right
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, this->rotation.axis.x, glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, this->rotation.axis.y, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, this->rotation.axis.z, glm::vec3(0.0f, 0.0f, 1.0f));
    
    int model_loc = glGetUniformLocation(this->mesh_data->shader_program, "model");
    int view_loc = glGetUniformLocation(this->mesh_data->shader_program, "view");
    int projection_loc = glGetUniformLocation(this->mesh_data->shader_program, "projection");

    glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection));

    
    glUseProgram(this->mesh_data->shader_program);

    glBindVertexArray(this->mesh_data->gl_VAO);

    glDrawElements(GL_TRIANGLES, this->mesh_data->indicies_size, GL_UNSIGNED_INT, 0);

    // glBindVertexArray(0);
}

vector<vec3> object::get_translation(vector<vec3> vertexes) {
    vector<vec3> ret_verts;
    vec3 pos = this->position;
    for (vec3 vert : vertexes) {
        ret_verts.push_back(vert + pos);
    }
    return ret_verts;
}

template<typename T>
void print_vec(vector<T> in) {
    for (T thing : in)
        std::cout << thing << ", ";
    std::cout << "|||endvec\n";
}