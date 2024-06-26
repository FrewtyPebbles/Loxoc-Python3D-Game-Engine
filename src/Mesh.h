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

using std::vector;
using std::string;
using std::map;
class mesh_dict;

enum illum_model {
    CONSTANT_COLOR,
    DIFFUSE,
    DIFFUSE_AND_SPECULAR
};

struct mesh_material {
    mesh_material(){};
    string name;
    tup<float, 3> ambient, diffuse, specular, emissive_coeficient;
    float specular_exponent, optical_density, transparency;
    illum_model illumination_model;
    string ambient_tex_file, diffuse_tex_file, specular_highlight_file;
    unsigned char * ambient_texture, diffuse_texture, specular_highlight_texture;
};

typedef RC<texture*>* rc_texture; 

class mesh {
public:
    mesh(){}
    mesh(const mesh& rhs):
    name(rhs.name),
    materials(rhs.materials),
    vertexes(rhs.vertexes),
    diffuse_coordinates(rhs.diffuse_coordinates),
    vertex_normals(rhs.vertex_normals),
    faces(rhs.faces),
    transform(rhs.transform),
    diffuse_textures(rhs.diffuse_textures),
    specular_textures(rhs.specular_textures),
    normals_textures(rhs.normals_textures)
    {}
    mesh(
        string name,
        vector<mesh_material*> materials,
        vector<vec3>* vertexes,
        vector<vec3>* diffuse_coordinates,
        vector<vec3>* vertex_normals,
        vector<tup<unsigned int, 3>>* faces,
        vec3 transform,
        vector<rc_texture> diffuse_textures,
        vector<rc_texture> specular_textures,
        vector<rc_texture> normals_textures
    ):
    name(name),
    materials(materials),
    vertexes(vertexes),
    diffuse_coordinates(diffuse_coordinates),
    vertex_normals(vertex_normals),
    faces(faces),
    transform(transform),
    diffuse_textures(diffuse_textures),
    specular_textures(specular_textures),
    normals_textures(normals_textures)
    {
        this->create_VAO();
    }
    ~mesh(){
        glDeleteVertexArrays(1, &gl_VAO);
        glDeleteBuffers(1, &gl_VBO);
        glDeleteBuffers(1, &gl_EBO);
        delete vertexes;
        delete diffuse_coordinates;
        delete vertex_normals;
        delete faces;
        for (auto mat : materials) {
            delete mat;
        }
    }
    static mesh_dict from_file(string file_path);
    string name;
    vector<mesh_material*> materials;
    // VVV THESE SHOULD BE HEAP ALLOCATED
    vector<vec3>* vertexes;
    vector<vec3>* diffuse_coordinates;
    vector<vec3>* vertex_normals;
    vector<tup<unsigned int, 3>>* faces;
    vec3 transform;
    vector<rc_texture> diffuse_textures;
    vector<rc_texture> specular_textures;
    vector<rc_texture> normals_textures;

    void get_gl_verts(vector<vec3> vertexes, vector<float>* mut_verts);
    void get_gl_vert_inds(vector<vec3> vertexes, vector<unsigned int>* mut_inds);

    unsigned int gl_VAO, gl_VBO, gl_EBO;
    size_t indicies_size;
    size_t verticies_size;
private:
    // RETURNS A HEAP ALLOCATED POINTER
    static void process_node(aiNode* node, const aiScene* scene, mesh_dict& meshes, const aiMatrix4x4& transform, string file_path);
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

typedef RC<mesh*>* rc_mesh;

class mesh_dict {
public:
    typedef typename std::map<string, vector<rc_mesh>>::iterator meshmap_iterator;
    typedef typename std::map<string, vector<rc_mesh>>::const_iterator const_meshmap_iterator;

    mesh_dict(){}
    mesh_dict(std::map<string, vector<rc_mesh>> data):data(data){}
    mesh_dict(const mesh_dict& rhs) : data(rhs.data) {}
    inline void insert(rc_mesh m) {
        if (this->data.contains(m->data->name))
            this->data[m->data->name].push_back(m);
        else {
            this->data.insert_or_assign(m->data->name, vector({m}));
        }
    }
    inline vector<rc_mesh> get(string name) {
        return this->data[name];
    }
    inline void remove(string name) {
        this->data.erase(name);
    }
    inline vector<rc_mesh> operator[](string name) {
        return this->data[name];
    }
    inline meshmap_iterator begin() noexcept { return this->data.begin(); }
    inline const_meshmap_iterator cbegin() const noexcept { return this->data.cbegin(); }
    inline meshmap_iterator end() noexcept { return this->data.end(); }
    inline const_meshmap_iterator cend() const noexcept { return this->data.cend(); }
    std::map<string, vector<rc_mesh>> data;
};


// Texture assimp macro

#define get_textures(type_name, aitype) vector<RC<texture*>*> type_name##_textures;\
        for (size_t t_n = 0; t_n < ai_mat->GetTextureCount(aitype); t_n++) {\
            aiString path;\
            if (ai_mat->GetTexture(aitype, t_n, &path) == AI_SUCCESS) {\
                if (str_tool::rem_path_from_file(string(path.C_Str())).find(".") != std::string::npos) {\
                    try {\
                        type_name##_textures.push_back(\
                            new RC(new texture(fix_texture_path(file_path, string(path.C_Str())), TextureWraping::REPEAT, TextureFiltering::LINEAR))\
                        );\
                    } catch ( std::runtime_error e ) {\
                        std::cerr << e.what();\
                    }\
                }\
            } else {\
                std::cerr << "Assimp failed to get "#type_name"s texture for node \"" << node->mName.C_Str() << "\"\n";\
            }\
        }