#include "Colliders.h"
#include "Object3d.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <algorithm>

std::pair<vec3, vec3> collider_box::transform_bounds() {
    auto upperv4 = owner->model_matrix * vec4(upper_bounds, 1.0);
    auto lowerv4 = owner->model_matrix * vec4(lower_bounds, 1.0);
    return std::make_pair(vec3(upperv4), vec3(lowerv4));
}

collider_box::collider_box(object3d* owner) {
    this->owner = owner;
    vec3 box_max = vec3(0,0,0);
    vec3 box_min = vec3(0,0,0);
    collider_box::mutate_max_min(owner->mesh_data->data, &box_max, &box_min);
    this->upper_bounds = box_max;
    this->lower_bounds = box_min;
    this->bounds[0] = upper_bounds;
    this->bounds[1] = vec3(lower_bounds.axis.x, upper_bounds.axis.y, upper_bounds.axis.z);
    this->bounds[2] = vec3(lower_bounds.axis.x, lower_bounds.axis.y, upper_bounds.axis.z);
    this->bounds[3] = vec3(upper_bounds.axis.x, lower_bounds.axis.y, upper_bounds.axis.z);
    this->bounds[4] = lower_bounds;
    this->bounds[5] = vec3(upper_bounds.axis.x, lower_bounds.axis.y, lower_bounds.axis.z);
    this->bounds[6] = vec3(upper_bounds.axis.x, upper_bounds.axis.y, lower_bounds.axis.z);
    this->bounds[7] = vec3(lower_bounds.axis.x, upper_bounds.axis.y, lower_bounds.axis.z);
}

void collider_box::mutate_max_min(mesh_dict* m_d, vec3* aabb_max, vec3* aabb_min) {
    for (auto [_, m] : *m_d) {
        if (std::holds_alternative<rc_mesh>(m)) {
            auto msh = std::get<rc_mesh>(m);
            aabb_max->axis.x = std::max(aabb_max->axis.x, msh->data->aabb_max.axis.x);
            aabb_max->axis.y = std::max(aabb_max->axis.y, msh->data->aabb_max.axis.y);
            aabb_max->axis.z = std::max(aabb_max->axis.z, msh->data->aabb_max.axis.z);

            aabb_min->axis.x = std::min(aabb_min->axis.x, msh->data->aabb_min.axis.x);
            aabb_min->axis.y = std::min(aabb_min->axis.y, msh->data->aabb_min.axis.y);
            aabb_min->axis.z = std::min(aabb_min->axis.z, msh->data->aabb_min.axis.z);
        } else {
            auto msh = std::get<rc_mesh_dict>(m);
            collider_box::mutate_max_min(msh->data, aabb_max, aabb_min);
        }
    }
}

bool collider_box::check_collision(vec3 intersection) {
    intersection = vec3(owner->model_matrix.inverse() * vec4(intersection.axis, 1.0));
    return upper_bounds >= intersection && lower_bounds <= intersection;
}


bool collider_box::check_collision(collider* other) {
    if (auto box = dynamic_cast<collider_box*>(other)) {
        return check_collision(box);
    }
    // Add other collider types here as needed
    return false;
}

bool collider_box::check_collision(collider_box* other) {
    vec3 dirs_this[3] = {
        this->owner->model_matrix[0],
        this->owner->model_matrix[1],
        this->owner->model_matrix[2]
    };

    vec3 dirs_other[3] = {
        other->owner->model_matrix[0],
        other->owner->model_matrix[1],
        other->owner->model_matrix[2]
    };

    // Check face normals as potential separating axes
    for (int i = 0; i < 3; ++i) {
        vec3 axis_this = dirs_this[i].get_normalized();
        if (!this->check_SAT(axis_this, other)) {
            return false;
        }

        vec3 axis_other = dirs_other[i].get_normalized();
        if (!other->check_SAT(axis_other, this)) {
            return false;
        }
    }

    return true;
}

std::pair<float, float> collider::minmax_vertex_SAT(const vec3 & axis) {
    if (auto box = dynamic_cast<collider_box*>(this)) {
        return box->minmax_vertex_SAT(axis);
    }
    // Add other collider types here as needed
    return std::make_pair(0.0f,0.0f);
}


std::pair<float, float> collider_box::minmax_vertex_SAT(const vec3 & axis) {
    float min_proj = vec3(this->owner->model_matrix * vec4(this->bounds[0].axis, 1.0f)).dot(axis.axis);
    float max_proj = min_proj;

    for (const auto &bound : this->bounds) {
        float proj = vec3(this->owner->model_matrix * vec4(bound.axis, 1.0f)).dot(axis);
        min_proj = std::min(min_proj, proj);
        max_proj = std::max(max_proj, proj);
    }

    return std::make_pair(min_proj, max_proj);
}

bool collider::check_SAT(vec3 axis, collider *other) {
    auto mm_this = this->minmax_vertex_SAT(axis);
    auto mm_other = other->minmax_vertex_SAT(axis);

    return !(mm_this.second < mm_other.first || mm_other.second < mm_this.first);
}