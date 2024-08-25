#include "Colliders.h"
#include "Object3d.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <algorithm>
#include "Tup.h"
#include <set>
#include "util.h"
#include "Camera.h"
#include "Model.h"

using std::set;

collider_box::collider_box(object3d* owner, vec3* offset, quaternion* rotation, vec3* scale) {
    this->owner = owner;
    vec3 box_max = vec3(0,0,0);
    vec3 box_min = vec3(0,0,0);
    collider_box::mutate_max_min(this->owner->model_data->data->mesh_data->data, &box_max, &box_min);
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
    this->offset = offset;
    this->rotation = rotation;
    this->scale = scale;
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
    auto this_mat = (this->owner->model_matrix.scale(this->scale) * matrix4x4(this->rotation)).translate(this->offset);


    intersection = (vec3(
        this->owner ?
        this_mat.inverse() * vec4(intersection.axis, 1.0)
        :
        vec4(intersection.axis, 1.0)
    ) - *offset);
    return upper_bounds >= intersection && lower_bounds <= intersection;
}


bool collider_box::check_collision(collider* other) {
    if (auto box = dynamic_cast<collider_box*>(other)) {
        return check_collision(box);
    } else if (auto convex = dynamic_cast<collider_convex*>(other)) {
        return check_collision(convex);
    }
    // Add other collider types here as needed
    return false;
}

bool collider_box::check_collision(collider_box* other) {
    auto this_mat = (this->owner->model_matrix.scale(this->scale) * matrix4x4(this->rotation)).translate(this->offset);
    auto other_mat = (other->owner->model_matrix.scale(other->scale) * matrix4x4(other->rotation)).translate(other->offset);

    vec3 dirs_this[3] = {
        this->owner ? this_mat[0] : vec3(1.0f, 0.0f, 0.0f),
        this->owner ? this_mat[1] : vec3(0.0f, 1.0f, 0.0f),
        this->owner ? this_mat[2] : vec3(0.0f, 0.0f, 1.0f)
    };

    vec3 dirs_other[3] = {
        other->owner ? other_mat[0] : vec3(1.0f, 0.0f, 0.0f),
        other->owner ? other_mat[1] : vec3(0.0f, 1.0f, 0.0f),
        other->owner ? other_mat[2] : vec3(0.0f, 0.0f, 1.0f)
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

bool collider_box::check_collision(collider_convex* other) {
    auto this_mat = (this->owner->model_matrix.scale(this->scale) * matrix4x4(this->rotation)).translate(this->offset);

    vec3 dirs_this[3] = {
        this->owner ? this_mat[0] : vec3(1.0f, 0.0f, 0.0f),
        this->owner ? this_mat[1] : vec3(0.0f, 1.0f, 0.0f),
        this->owner ? this_mat[2] : vec3(0.0f, 0.0f, 1.0f)
    };

    // Check face normals as potential separating axes
    for (int i = 0; i < 3; ++i) {
        vec3 axis_this = dirs_this[i].get_normalized();
        if (!this->check_SAT(axis_this, other)) {
            return false;
        }
    }
    
    for (auto face : other->hull) {
        if (!other->check_SAT(face.normal, this)) {
            return false;
        }
    }

    return true;
}

std::pair<float, float> collider::minmax_vertex_SAT(const vec3 & axis) {
    if (auto box = dynamic_cast<collider_box*>(this)) {
        return box->minmax_vertex_SAT(axis);
    } else if (auto convex = dynamic_cast<collider_convex*>(this)) {
        return convex->minmax_vertex_SAT(axis);
    }
    // Add other collider types here as needed
    return std::make_pair(0.0f,0.0f);
}


std::pair<float, float> collider_box::minmax_vertex_SAT(const vec3 & axis) {
    auto this_mat = (this->owner->model_matrix.scale(this->scale) * matrix4x4(this->rotation)).translate(this->offset);
    float min_proj = (vec3(
        this->owner ?
        this_mat * vec4(this->bounds[0], 1.0f)
        :
        vec4(this->bounds[0], 1.0f)
    ) + *offset).dot(axis);
    float max_proj = min_proj;

    for (const auto &bound : this->bounds) {
        float proj = (vec3(
            this->owner ?
            this_mat * vec4(bound, 1.0f)
            :
            vec4(bound, 1.0f)
        ) + *offset).dot(axis);
        min_proj = std::min(min_proj, proj);
        max_proj = std::max(max_proj, proj);
    }

    return std::make_pair(min_proj, max_proj);
}

bool collider::check_SAT(vec3 axis, collider *other) {
    auto [min1, max1] = this->minmax_vertex_SAT(axis);
    auto [min2, max2] = other->minmax_vertex_SAT(axis);

    return max1 >= min2 && max2 >= min1;
}

collider_convex::collider_convex(object3d* owner, vec3* offset, quaternion* rotation, vec3* scale) {
    this->owner = owner;
    this->offset = offset;
    generate_hull(this->owner->model_data->data->mesh_data->data->gather_mesh_verticies());
    render_hull_create_shader_program();
    this->offset = offset;
    this->rotation = rotation;
    this->scale = scale;
}

collider_convex::collider_convex(rc_mesh owner, vec3* offset, quaternion* rotation, vec3* scale) {
    vector<vec3> verts;
    for (const auto & v : *owner->data->vertices)
        verts.push_back(v.position);
    generate_hull(verts);
    this->offset = offset;
    render_hull_create_shader_program();
    this->offset = offset;
    this->rotation = rotation;
    this->scale = scale;
}

collider_convex::collider_convex(rc_mesh_dict owner, vec3* offset, quaternion* rotation, vec3* scale) {
    generate_hull(owner->data->gather_mesh_verticies());
    this->offset = offset;
    render_hull_create_shader_program();
    this->offset = offset;
    this->rotation = rotation;
    this->scale = scale;
}

void collider_convex::render_hull_extract_edges() {
    vector<vec3> uniqueVertices;

    // Add unique vertices
    for (const auto& face : hull) {
        for (int i = 0; i < 3; ++i) {
            raw_vertices.push_back(face.vertices[i].axis.x);
            raw_vertices.push_back(face.vertices[i].axis.y);
            raw_vertices.push_back(face.vertices[i].axis.z);
        }
    }
}

void collider_convex::render_hull_create_shader_program() {
    unsigned int vertexShader, fragmentShader;
    // Vertex Shader source code
    const char* vertexShaderSource = R"(
        #version 450 core
        layout(location = 0) in vec3 aPos;
        out vec3 pos;
        out vec3 frag_pos;

        uniform mat4 transform;
        uniform mat4 model;

        void main() {
            gl_Position = transform * vec4(aPos, 1.0);
            pos = vec3(gl_Position);
            frag_pos = vec3(model * vec4(aPos, 1.0));
        }
    )";

    // Fragment Shader source code
    const char* fragmentShaderSource = R"(
        #version 450 core
        out vec4 FragColor;
        in vec3 pos;
        in vec3 frag_pos;
        void main() {
            FragColor = vec4(0.0, 1.0, 0.0, 0.5); // Green color
        }
    )";
    
    // Vertex Shader
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    
    // Fragment Shader
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    
    // Shader Program
    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertexShader);
    glAttachShader(shader_program, fragmentShader);
    glLinkProgram(shader_program);

    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "ERROR: Vertex Shader Compilation Failed\n" << infoLog << std::endl;
    }   
    
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "ERROR: Fragment Shader Compilation Failed\n" << infoLog << std::endl;
    } 
    
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_program, 512, NULL, infoLog);
        std::cerr << "ERROR: Shader Program Linking Failed\n" << infoLog << std::endl;
    }  
     
    // Clean up shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    // Extract edges from faces
    render_hull_extract_edges();
    
    // Generate and bind VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO); 

    // Generate and bind VBO for vertices
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, raw_vertices.size() * sizeof(float), raw_vertices.data(), GL_STATIC_DRAW);

    // Set vertex attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}
 
void collider_convex::render_hull(const camera& cam) {
    if (show_collider) {
        auto this_mat = (this->owner->model_matrix.scale(this->scale) * matrix4x4(this->rotation)).translate(this->offset);
        glDepthMask(GL_FALSE); 
        // Use shader program
        glUseProgram(shader_program); 

        auto t_loc = glGetUniformLocation(shader_program, "transform");

        glUniformMatrix4fv(t_loc, 1, GL_FALSE, glm::value_ptr(cam.projection.mat * cam.view.mat * this_mat.mat));

        auto t2_loc = glGetUniformLocation(shader_program, "model");

        glUniformMatrix4fv(t2_loc, 1, GL_FALSE, glm::value_ptr(this_mat.mat));
 
        // Draw the wireframe
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, (GLint)raw_vertices.size() / 3);
        glBindVertexArray(0);
        glDepthMask(GL_TRUE);
    } 
}      
 
float collider_convex::tetrahedron_volume(const vec3& p1, const vec3& p2, const vec3& p3, const vec3& p4) {
    vec3 v1 = p2 - p1;
    vec3 v2 = p3 - p1;
    vec3 v3 = p4 - p1;
    return std::fabs(v1.dot(v2.cross(v3))) / 6.0f;
}  


std::vector<hull_face> collider_convex::find_tetrahedron(const std::vector<vec3>& vertices) {
    if (vertices.size() < 4) {
        throw std::runtime_error("Insufficient vertices to form a tetrahedron.");
    }

    vec3 bestTetrahedron[4];
    float maxVolume = -std::numeric_limits<float>::max();
    bool foundTetrahedron = false;

    // Step 1: Find the first valid triangle
    for (size_t i = 0; i < vertices.size() - 2; ++i) {
        for (size_t j = i + 1; j < vertices.size() - 1; ++j) {
            for (size_t k = j + 1; k < vertices.size(); ++k) {
                vec3 normal = (vertices[j] - vertices[i]).cross(vertices[k] - vertices[i]);
                if (normal.get_magnitude() > std::numeric_limits<float>::epsilon()) { // Check if the triangle is not degenerate
                    // Step 2: Find a fourth vertex that does not lie in the plane of the triangle
                    for (size_t l = 0; l < vertices.size(); ++l) {
                        if (l == i || l == j || l == k) continue;
                        
                        float volume = tetrahedron_volume(vertices[i], vertices[j], vertices[k], vertices[l]);
                        if (volume > std::numeric_limits<float>::min()) {
                            if (volume > maxVolume) {
                                maxVolume = volume;
                                bestTetrahedron[0] = vertices[i];
                                bestTetrahedron[1] = vertices[j];
                                bestTetrahedron[2] = vertices[k];
                                bestTetrahedron[3] = vertices[l];
                                foundTetrahedron = true;
                            }
                        }
                    }
                }
                if (foundTetrahedron) break;
            }
            if (foundTetrahedron) break;
        }
        if (foundTetrahedron) break;
    }

    // Step 3: Return the faces of the best tetrahedron found
    if (foundTetrahedron) {
        return {
            hull_face(bestTetrahedron[0], bestTetrahedron[1], bestTetrahedron[2]),
            hull_face(bestTetrahedron[0], bestTetrahedron[1], bestTetrahedron[3]),
            hull_face(bestTetrahedron[0], bestTetrahedron[2], bestTetrahedron[3]),
            hull_face(bestTetrahedron[1], bestTetrahedron[2], bestTetrahedron[3])
        };
    } else {
        throw std::runtime_error("Failed to find a valid non-planar, non-hyperplanar tetrahedron.");
    }
}

vec3 collider_convex::calculate_midpoint(const set<vec3, vec3_compare>& points) {
    if (points.empty()) {
        throw std::invalid_argument("The list of points cannot be empty.");
    }
    
    vec3 sum(0.0f, 0.0f, 0.0f);
    
    for (const auto& point : points) {
        sum += point;
    }
    
    return sum / static_cast<float>(points.size());
}

void collider_convex::flip_normals(const vec3& center) {
    for (auto& face : hull)
        if (face.distance(center) > 0.0f)
            face.normal = -face.normal;
}

void collider_convex::generate_hull(vector<vec3> _vertices)  {

    set<vec3, vec3_compare> t_verts;
    for (auto v : _vertices)
        t_verts.insert(v);

    vector<vec3> vertices;
    for (auto v : t_verts)
        vertices.push_back(v);

    if (vertices.size() < 4) {
        throw std::runtime_error("Mesh must have at least 4 verticies to generate a ConvexCollider.");
        return;
    }

    auto faces = find_tetrahedron(vertices);
    // Possible solution:
    // create a vertex that is in the center of the tetrahedron.
    // project(dot product) that vertex onto the normal of all faces.
    // if any of the faces have a dot product with the normal greater than 0, negate the normal.
    // greater than 0 means that the normal is inverted.
    set<vec3, vec3_compare> face_points;
    for (auto& face : faces) {
        face_points.insert(face.vertices[0]);
        face_points.insert(face.vertices[1]);
        face_points.insert(face.vertices[2]);
    }
        
    vec3 center = calculate_midpoint(face_points);



    hull.insert(hull.end(), faces.begin(), faces.end());

    while (true) {

        flip_normals(center);

        for (auto v : vertices)
            for (auto& face : hull)
                if (face.is_visible(v))
                    face.outside.insert(v);
        
        for (auto it = hull.begin(); it != hull.end();) {
                
            
            if (it->outside.empty()) {
                ++it;
                continue;
            }

            vec3 furthest = it->furthest_outside();

            auto vis_set = create_visible_set(furthest);


            auto new_faces = create_faces_from_point(vis_set, furthest, &vertices);

            hull.insert(hull.end(), new_faces.begin(), new_faces.end());

            hull.erase(std::remove_if(hull.begin(), hull.end(), [&vis_set](const hull_face& element) {
                return vis_set.contains(element);
            }), hull.end());

            break;
        }

        bool finished = true;
        for (auto v : vertices)
            for (auto& face : hull)
                if (face.is_visible(v))
                    finished = false;
        if (finished) break;
    }
}

vec3 hull_face::furthest_outside() {
    // Ensure the outside vector is not empty
    if (outside.empty()) {
        throw std::runtime_error("No vertices in the outside vector.");
    }

    auto max = *outside.begin(); // Initialize with the first vertex
    float max_distance = distance(max); // Compute the distance for the first vertex

    for (const auto& v : outside) {
        float current_distance = distance(v);
        if (current_distance > max_distance) {
            max_distance = current_distance;
            max = v;
        }
    }

    return max;
}

vector<hull_face> collider_convex::create_faces_from_point(const set<hull_face>& vis_set, const vec3& farthest_point, std::vector<vec3>* vertices) {
    auto horizon_edges = identify_horizon_edges(vis_set);

    vector<hull_face> ret;
    for (auto [v1, v2] : horizon_edges) {
        ret.push_back(hull_face(farthest_point, v1, v2));
    }
    return ret;
}

vector<pair<vec3, vec3>> collider_convex::identify_horizon_edges(set<hull_face> visible_set) {
    map<pair<vec3, vec3>, int, vec3_pair_compare> edgeCount; // To count occurrences of edges

    // Count how often each edge appears in the visible set
    for (const auto& face : visible_set) {
        for (int i = 0; i < 3; i++) {
            vec3 v1 = face.vertices[i];
            vec3 v2 = face.vertices[(i + 1) % 3];
            if (edgeCount.contains({v2, v1}))
                edgeCount[{v2, v1}]++;
            else
                edgeCount[{v1, v2}]++;
        }
    }

    vector<pair<vec3, vec3>> horizon_edges;

    // An edge is a horizon edge if it appears exactly once in the visible set
    for (const auto& [edge, count] : edgeCount) {
        if (count == 1) {
            horizon_edges.push_back(edge);
        }
    }

    return horizon_edges;
}


set<hull_face> collider_convex::create_visible_set(const vec3& point) {
    set<hull_face> visible;
    for (auto face : hull) {
        if (face.is_visible(point))
            visible.insert(face);
    }
    return visible;
}


vec3 calculate_normal(const vec3& v0, const vec3& v1, const vec3& v2) {
    return (v1 - v0).cross(v2 - v0).get_normalized();
    // since the cross product is the orthagonal vector of 2 vectors this gives you the normal
}

float hull_face::distance(const vec3& point) const {
    return normal.dot(point - vertices[0]);
    // projects the vector from a vert of the face to the point onto the normal
    // if the vector is outside the face it will project a positive distance onto the normal
    // if it is inside the face it will project a negative 
}

bool hull_face::is_visible(const vec3& point) const {
    return distance(point) > std::numeric_limits<float>::min() && !contains(point);
}

// convex collisions:

bool collider_convex::check_collision(vec3 intersection) {
    auto this_mat = (this->owner->model_matrix.scale(this->scale) * matrix4x4(this->rotation)).translate(this->offset);
    vec3 transformed_point = this->owner ? this_mat.inverse() * vec4(intersection, 1.0f) : vec4(intersection, 1.0f);
    for (const auto& face : hull) {
        if (!face.is_visible(transformed_point - *offset)) {
            return false;
        }
    }
    return true;
}

bool collider_convex::check_collision(collider* other) {
    if (auto box = dynamic_cast<collider_box*>(other)) {
        return check_collision(box);
    } else if (auto convex = dynamic_cast<collider_convex*>(other)) {
        return check_collision(convex);
    }
    // Add other collider types here as needed
    return false;
}

bool collider_convex::check_collision(collider_box* other) {
    auto other_mat = (other->owner->model_matrix.scale(other->scale) * matrix4x4(other->rotation)).translate(other->offset);
    vec3 dirs_other[3] = {
        other->owner ? other_mat[0] : vec3(1.0f, 0.0f, 0.0f),
        other->owner ? other_mat[1] : vec3(0.0f, 1.0f, 0.0f),
        other->owner ? other_mat[2] : vec3(0.0f, 0.0f, 1.0f)
    };

    // Check face normals as potential separating axes
    for (auto face : hull) {
        if (!this->check_SAT(face.normal, other)) {
            return false;
        }
    }
        
    for (int i = 0; i < 3; ++i) {
        vec3 axis_other = dirs_other[i].get_normalized();
        if (!other->check_SAT(axis_other, this)) {
            return false;
        }
    }

    return true;
}

bool collider_convex::check_collision(collider_convex* other) {

    // Check face normals as potential separating axes
    for (auto face : hull) {
        if (!this->check_SAT(face.normal, other)) {
            return false;
        }
    }
        
    for (auto face : other->hull) {
        if (!other->check_SAT(face.normal, this)) {
            return false;
        }
    }

    return true;
}

std::pair<float, float> collider_convex::minmax_vertex_SAT(const vec3 & axis) {
    auto this_mat = (this->owner->model_matrix.scale(this->scale) * matrix4x4(this->rotation)).translate(this->offset);
    float min_proj = axis.dot(vec3(
        this->owner ?
        this_mat * vec4(this->hull[0].vertices[0], 1.0f)
        :
        vec4(this->hull[0].vertices[0], 1.0f)
        ) + *offset);
    float max_proj = min_proj;

    for (const auto &face : this->hull) {
        for (const auto& vertex : face.vertices) {
            float proj = axis.dot(vec3(
                this->owner ?
                this_mat * vec4(vertex, 1.0f)
                :
                vec4(vertex, 1.0f)
            ) + *offset);
            if (proj < min_proj) min_proj = proj;
            if (proj > max_proj) max_proj = proj;
        }
    }

    return std::make_pair(min_proj, max_proj);
}