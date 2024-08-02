#pragma once
#include "Vec3.h"
#include <utility>
#include "Mesh.h"


class object3d;

class collider {
public:
    collider() {}
    virtual bool check_collision(vec3 intersection) = 0;
    virtual bool check_collision(collider* intersection) = 0;
    virtual std::pair<float, float> minmax_vertex_SAT(const vec3 & axis) = 0;
    bool check_SAT(vec3 axis, collider* other); // Separating Axis Theorem
    object3d* owner;
};

class collider_box : public collider {
public:
    using collider::check_collision;
    collider_box() {}
    collider_box(object3d* owner);
    collider_box(vec3 upper_bounds, vec3 lower_bounds) :
        upper_bounds(upper_bounds),
        lower_bounds(lower_bounds),
        bounds{
            upper_bounds,
            vec3(lower_bounds.axis.x, upper_bounds.axis.y, upper_bounds.axis.z),
            vec3(lower_bounds.axis.x, lower_bounds.axis.y, upper_bounds.axis.z),
            vec3(upper_bounds.axis.x, lower_bounds.axis.y, upper_bounds.axis.z),
            lower_bounds,
            vec3(upper_bounds.axis.x, lower_bounds.axis.y, lower_bounds.axis.z),
            vec3(upper_bounds.axis.x, upper_bounds.axis.y, lower_bounds.axis.z),
            vec3(lower_bounds.axis.x, upper_bounds.axis.y, lower_bounds.axis.z)
        }
    {};
    bool check_collision(vec3 intersection) override;
    bool check_collision(collider* other) override;
    bool check_collision(collider_box* collider);

    std::pair<float, float> minmax_vertex_SAT(const vec3 & axis) override;
    vec3 upper_bounds;
    vec3 lower_bounds;
    vec3 bounds[8];
private:
    std::pair<vec3, vec3> transform_bounds();
    static void mutate_max_min(mesh_dict* m, vec3* aabb_max, vec3* aabb_min);
};

