#include "Camera.h"
#include "Vec3.h"
#include "Object.h"
#include <algorithm>


void camera::render(vector<object*> objects) {
    for (object * obj : objects) {
        obj->render(*this);
    }
}

camera::camera() {}

camera::~camera() {}


camera::camera(vec3* position, int view_width, int view_height, float focal_length, float fov) {
    this->position = position;
    this->view_width = view_width;
    this->view_height = view_height;
    this->focal_length = focal_length;
    this->fov = fov;

}  