#pragma once
#include "Vec3.h"
#include <utility>
#include "Mesh.h"
#include "Matrix.h"
#include "Vec4.h"
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <set>
#include <initializer_list>
#include "Quaternion.h"

using std::set;
using std::pair;
using std::queue;

class object3d;
class collider_convex;

class collider {
public:
    collider() {}
    virtual bool check_collision(vec3 intersection) = 0;
    virtual bool check_collision(collider* intersection) = 0;
    virtual std::pair<float, float> minmax_vertex_SAT(const vec3 & axis) = 0;
    bool check_SAT(vec3 axis, collider* other); // Separating Axis Theorem
    object3d* owner = nullptr;
    vec3* offset;
    vec3* scale;
    quaternion* rotation;
    bool show_collider = false;
};

class collider_box : public collider {
public:
    using collider::check_collision;
    collider_box() {}
    collider_box(object3d* owner, vec3* offset, quaternion* rotation, vec3* scale);
    collider_box(vec3 upper_bounds, vec3 lower_bounds, vec3* offset, quaternion* rotation, vec3* scale) :
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
    {this->offset = offset;};
    bool check_collision(vec3 intersection) override;
    bool check_collision(collider* other) override;
    bool check_collision(collider_box* collider);
    bool check_collision(collider_convex* collider);

    std::pair<float, float> minmax_vertex_SAT(const vec3 & axis) override;
    vec3 upper_bounds;
    vec3 lower_bounds;
    vec3 bounds[8];
private:
    static void mutate_max_min(mesh_dict* m, vec3* aabb_max, vec3* aabb_min);
};

vec3 calculate_normal(const vec3& a, const vec3& b, const vec3& c);

inline bool lt_vec(const vec3& lhs, const vec3& rhs) {
    if (lhs.axis.x != rhs.axis.x) return lhs.axis.x < rhs.axis.x;
    if (lhs.axis.y != rhs.axis.y) return lhs.axis.y < rhs.axis.y;
    return lhs.axis.z < rhs.axis.z;
}

class hull_face {
public:
    hull_face(vec3 v1, vec3 v2, vec3 v3):
    normal(calculate_normal(v1, v2, v3)),
    vertices{v1, v2, v3}
    {
        if (!hull_face::validate_face(v1, v2, v3)) {
            std::cout << "DUPLICATE VERT IN FACE.\n";
            throw std::runtime_error("DUPLICATE VERT IN FACE.");
        }
    }

    hull_face(vec3 normal, vec3 v1, vec3 v2, vec3 v3):
    normal(normal),
    vertices{v1, v2, v3}
    {
        if (!hull_face::validate_face(v1, v2, v3)) {
            std::cout << "DUPLICATE VERT IN FACE.\n";
            throw std::runtime_error("DUPLICATE VERT IN FACE.");
        }
    }

    inline static bool validate_face(vec3 v1, vec3 v2, vec3 v3) {
        return !(v1 == v2 || v2 == v3 || v3 == v1);
    }
    vec3 normal;
    vec3 vertices[3];
    set<vec3> outside;

    friend inline bool operator==(const hull_face& lhs, const hull_face& rhs) {
        // Check if each vertex in this face exists in the other face
        return lhs.vertices[0] == rhs.vertices[0] && lhs.vertices[1] == rhs.vertices[1] && lhs.vertices[2] == rhs.vertices[2];
    }
    friend inline bool operator<(const hull_face& lhs, const hull_face& rhs) {
        // Compare vertices lexicographically
        for (int i = 0; i < 3; ++i) {
            if (lt_vec(lhs.vertices[i], rhs.vertices[i])) return true;
            if (lt_vec(rhs.vertices[i], lhs.vertices[i])) return false;
        }
        return false; // They are equal
    }
    friend inline bool operator>(const hull_face& lhs, const hull_face& rhs) {
        // Check if each vertex in this face exists in the other face
        return lhs.vertices[0] > rhs.vertices[0] && lhs.vertices[1] > rhs.vertices[1] && lhs.vertices[2] > rhs.vertices[2];
    }
    inline bool contains(const vec3& vec) const {
        return vertices[0] == vec || vertices[1] == vec || vertices[2] == vec;
    }
    friend inline std::ostream& operator<<(std::ostream& os, const hull_face& self){
        os << "face[" << self.vertices[0] << ", " << self.vertices[1] << ", " << self.vertices[2] << ']';
        return os;
    }
    bool is_visible(const vec3& point) const;
    float distance(const vec3& point) const;
    vec3 furthest_outside();
};
struct vec3_compare;
class collider_convex : public collider {
public:
    using collider::check_collision;
    collider_convex() {}
    collider_convex(object3d* owner, vec3* offset, quaternion* rotation, vec3* scale);
    collider_convex(rc_mesh owner, vec3* offset, quaternion* rotation, vec3* scale);
    collider_convex(rc_mesh_dict owner, vec3* offset, quaternion* rotation, vec3* scale);
    bool check_collision(vec3 intersection) override;
    bool check_collision(collider* other) override;
    bool check_collision(collider_box* collider);
    bool check_collision(collider_convex* collider);

    std::pair<float, float> minmax_vertex_SAT(const vec3 & axis) override;
    vector<hull_face> hull;

    // DEBUG
    void render_hull(const camera& cam);
private:
    void generate_hull(vector<vec3> verticies);
    set<hull_face> create_visible_set(const vec3& point);
    vector<pair<vec3, vec3>> identify_horizon_edges(set<hull_face> visible_set);
    vec3 calculate_midpoint(const set<vec3, vec3_compare>& points);
    void flip_normals(const vec3& center);

    // debug
    void render_hull_extract_edges();
    void render_hull_create_shader_program();
    vector<hull_face> find_tetrahedron(const std::vector<vec3>& vertices);
    float tetrahedron_volume(const vec3& p1, const vec3& p2, const vec3& p3, const vec3& p4);
    vector<hull_face> create_faces_from_point(const set<hull_face>& vis_set, const vec3& farthest_point, std::vector<vec3>* vertices);
    unsigned int shader_program;
    unsigned int VAO, VBO;
    vector<float> raw_vertices;
};


struct vec3_pair_compare {
    inline bool operator()(const std::pair<vec3, vec3>& a, const std::pair<vec3, vec3>& b) const {
        if (a.first != b.first) return lt_vec(a.first, b.first);
        return lt_vec(a.second, b.second);
    }
};

struct vec3_compare {
    inline bool operator()(const vec3& a, const vec3& b) const {
        return lt_vec(a, b);
    }
};
