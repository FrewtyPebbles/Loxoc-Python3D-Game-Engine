#include "Camera.h"
#include "Vec3.h"
#include "Object3d.h"
#include <algorithm>


camera::camera() {}

camera::~camera() {}


camera::camera(vec3* position, quaternion* rotation, int view_width, int view_height, float focal_length, float fov) {
    this->position = position;
    this->rotation = rotation;
    this->view_width = view_width;
    this->view_height = view_height;
    this->focal_length = focal_length;
    this->fov = fov;
    this->projection = glm::perspective(this->fov, static_cast<float>(this->view_width)/static_cast<float>(this->view_height), 0.1f, static_cast<float>(this->focal_length));
    this->view = glm::lookAt(this->position->axis, this->position->axis + this->rotation->get_forward().axis, this->rotation->get_up().axis);
}

void camera::recalculate_pv() {
    this->projection = glm::perspective(this->fov, static_cast<float>(this->view_width)/static_cast<float>(this->view_height), 0.1f, static_cast<float>(this->focal_length));
    this->view = glm::lookAt(this->position->axis, this->position->axis + this->rotation->get_forward().axis, this->rotation->get_up().axis);
}