#pragma once
#include <vector>
#include <string>
#include <sstream>
#include "Tup.h"
#include <map>
#include "glad/gl.h"
#include "Shader.h"
#include "Vec3.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Texture.h"
#include <map>
#include <iterator>
#include "RC.h"
#include <variant>
#include "Material.h"
#include "util.h"

#define MAX_BONE_INFLUENCE 4

using std::vector;
using std::string;
using std::map;
class mesh_dict;
class mesh;
class model;

typedef RC<mesh*>* rc_mesh;

typedef RC<model*>* rc_model;

typedef RC<mesh_dict*>* rc_mesh_dict;

typedef std::variant<rc_mesh, rc_mesh_dict> mesh_dict_child;

enum illum_model {
    CONSTANT_COLOR,
    DIFFUSE,
    DIFFUSE_AND_SPECULAR
};

typedef RC<texture*>* rc_texture;

struct vertex {
    // position
    glm::vec3 position;
    // normal
    glm::vec3 normal;
    // texCoords
    glm::vec2 tex_coords;
	//bone indexes which will influence this vertex
	glm::ivec4 bone_ids;
	//weights from each bone
	glm::vec4 weights;
};

class mesh {
public:
    mesh(){}
    mesh(const mesh& rhs):
    name(rhs.name),
    mesh_material(rhs.mesh_material),
    vertices(rhs.vertices),
    faces(rhs.faces),
    transform(rhs.transform)
    {}
    mesh(
        string name,
        rc_material mesh_material,
        vector<vertex>* vertices,
        vector<tup<unsigned int, 3>>* faces,
        vec3 transform,
        bool is_animated = false
    ):
    name(name),
    mesh_material(mesh_material),
    vertices(vertices),
    faces(faces),
    transform(transform),
    is_animated(is_animated)
    {
        this->create_VAO();
    }
    ~mesh(){
        glDeleteVertexArrays(1, &gl_VAO);
        glDeleteBuffers(1, &gl_VBO);
        glDeleteBuffers(1, &gl_EBO);
        delete faces;
        delete vertices;
    }
    static rc_model from_file(string file_path, bool animated);
    string name;
    rc_material mesh_material;

    // VVV THESE SHOULD BE HEAP ALLOCATED
    vector<tup<unsigned int, 3>>* faces;
    vector<vertex>* vertices;
    bool is_animated = false;

    vec3 transform;
    float radius = 0;
    void get_gl_vert_inds(vector<unsigned int>* mut_inds);

    unsigned int gl_VAO, gl_VBO, gl_EBO;
    size_t indicies_size;
    vec3 aabb_max = vec3(0,0,0);
    vec3 aabb_min = vec3(0,0,0);
private:
    // RETURNS A HEAP ALLOCATED POINTER
    static void process_node(rc_model model, aiNode* node, const aiScene* scene, rc_mesh_dict last_mesh_dict, const aiMatrix4x4& transform, string file_path);
    void create_VAO();
};

inline string trim(const string& str)
{
    size_t first = str.find_first_not_of(' ');
    if (string::npos == first)
    {
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
} 
  
inline vector<std::string> split(std::string const &input) { 
    std::istringstream buffer(input);
    vector<std::string> ret((std::istream_iterator<std::string>(buffer)), 
                                 std::istream_iterator<std::string>());
    return ret;
}

inline size_t sub_str_ind(const std::string& haystack, const std::string& needle, int nth)
{
    size_t  pos = 0;
    int     cnt = 0;

    while( cnt != nth )
    {
        pos+=1;
        pos = haystack.find(needle, pos);
        if ( pos == std::string::npos )
            return -1;
        cnt++;
    }
    return pos;
}




class mesh_dict {
public:
    typedef typename std::map<string, mesh_dict_child>::iterator meshmap_iterator;
    typedef typename std::map<string, mesh_dict_child>::const_iterator const_meshmap_iterator;

    mesh_dict(){}
    mesh_dict(string name, std::map<string, mesh_dict_child> data):data(data), name(name){}
    mesh_dict(const mesh_dict& rhs) : data(rhs.data), name(rhs.name) {}
    inline void insert(mesh_dict_child m) {
        if (std::holds_alternative<rc_mesh>(m)) {
            auto msh = std::get<rc_mesh>(m);
            this->data.insert_or_assign(msh->data->name, msh);
        } else if (std::holds_alternative<rc_mesh_dict>(m)) {
            auto msh_d = std::get<rc_mesh_dict>(m);
            
            this->data.insert_or_assign(msh_d->data->name, msh_d);
        }
    }
    inline mesh_dict_child get(string name) {
        return this->data[name];
    }
    inline void remove(string name) {
        this->data.erase(name);
    }
    inline mesh_dict_child operator[](string name) {
        return this->data[name];
    }
    inline vector<vec3> gather_mesh_verticies() {
        vector<vec3> ret;
        for (auto [key, m] : *this) {
            if (std::holds_alternative<rc_mesh>(m)) {
                auto msh = std::get<rc_mesh>(m);
                // verticies
                for (const auto & v : *msh->data->vertices)
                    ret.push_back(v.position);
            } else if (std::holds_alternative<rc_mesh_dict>(m)) {
                auto msh_d = std::get<rc_mesh_dict>(m);
                auto m_data = msh_d->data->gather_mesh_verticies();
                vec_extend(ret, m_data);
            }
        }
        return ret;
    }
    inline meshmap_iterator begin() { return this->data.begin(); }
    inline const_meshmap_iterator cbegin() const { return this->data.cbegin(); }
    inline meshmap_iterator end() { return this->data.end(); }
    inline const_meshmap_iterator cend() const { return this->data.cend(); }
    std::map<string, mesh_dict_child> data;
    string name;
};


// Texture assimp macro

#define get_textures(type_name, aitype) for (size_t t_n = 0; t_n < ai_mat->GetTextureCount(aitype); t_n++) {\
            aiString path;\
            if (ai_mat->GetTexture(aitype, t_n, &path) == AI_SUCCESS) {\
                if (str_tool::rem_path_from_file(string(path.C_Str())).find(".") != std::string::npos) {\
                    try {\
                        mesh_material->data->type_name##_texture = new RC(new texture(fix_texture_path(file_path, string(path.C_Str())), TextureWraping::REPEAT, TextureFiltering::LINEAR));\
                    } catch ( std::runtime_error e ) {\
                        std::cerr << e.what();\
                    }\
                }\
            } else {\
                std::cerr << "Assimp failed to get "#type_name"s texture for node \"" << node->mName.C_Str() << "\"\n";\
            }\
        }