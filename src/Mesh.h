#pragma once
#include <vector>
#include <string>
#include <sstream>
#include "Tup.h"
#include <opencv2/opencv.hpp>
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

using std::vector;
using std::string;
using std::map;


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
    cv::Mat ambient_texture, diffuse_texture, specular_highlight_texture;
};


class mesh {
public:
    mesh(){}
    mesh(
        vector<mesh_material*> materials,
        vector<vec3>* vertexes,
        vector<vec3>* diffuse_coordinates,
        vector<vec3>* vertex_normals,
        vector<tup<unsigned int, 3>>* faces,
        vec3 transform
    ):
    materials(materials),
    vertexes(vertexes),
    diffuse_coordinates(diffuse_coordinates),
    vertex_normals(vertex_normals),
    faces(faces),
    transform(transform)
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
    static vector<mesh*> from_file(string file_path);
    vector<mesh_material*> materials;
    // VVV THESE SHOULD BE HEAP ALLOCATED
    vector<vec3>* vertexes;
    vector<vec3>* diffuse_coordinates;
    vector<vec3>* vertex_normals;
    vector<tup<unsigned int, 3>>* faces;
    vec3 transform;

    void get_gl_verts(vector<vec3> vertexes, vector<float>* mut_verts);
    void get_gl_vert_inds(vector<vec3> vertexes, vector<unsigned int>* mut_inds);

    unsigned int gl_VAO, gl_VBO, gl_EBO;
    size_t indicies_size;
    size_t verticies_size;
private:
    // RETURNS A HEAP ALLOCATED POINTER
    static void process_node(aiNode* node, const aiScene* scene, vector<mesh*>& meshes, const aiMatrix4x4& transform);
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

template<typename T>
void print_vec(vector<T> in);

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