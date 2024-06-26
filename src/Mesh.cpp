#include "Mesh.h"
#include "Vec3.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include "debug.h"
#include "util.h"
#include <filesystem>
#include <sstream>

string fix_texture_path(string file_path, string file) {
    return std::filesystem::absolute(std::filesystem::path(str_tool::rem_file_from_path(file_path) + "/textures/" + str_tool::rem_path_from_file(file))).string();
}

void mesh::process_node(aiNode* node, const aiScene* scene, mesh_dict& meshes, const aiMatrix4x4& transform, string file_path) {
    // traverse to deeper nodes
    for (size_t c_n = 0; c_n < node->mNumChildren; c_n++) {
        process_node(node->mChildren[c_n], scene, meshes, transform * node->mChildren[c_n]->mTransformation, file_path);
    }
    // itterate meshes for the node
    auto t_aivec3 = transform * aiVector3D(1.0f, 1.0f, 1.0f);
    for (size_t m_n = 0; m_n < node->mNumMeshes; m_n++) {
        auto msh = scene->mMeshes[node->mMeshes[m_n]];
        vector<mesh_material*> materials;
        vector<vec3>* _vertexes = new vector<vec3>();
        vector<vec3>* _diffuse_coordinates = new vector<vec3>();
        vector<vec3>* _vertex_normals = new vector<vec3>();
        vector<tup<unsigned int, 3>>* faces = new vector<tup<unsigned int, 3>>();
        vec3 _transform = vec3(t_aivec3.x, t_aivec3.y, t_aivec3.z);
        // get material data
        auto ai_mat = scene->mMaterials[msh->mMaterialIndex];
        // get texture data
    
        get_textures(diffuse, aiTextureType_DIFFUSE);
        get_textures(specular, aiTextureType_SPECULAR);
        get_textures(normals, aiTextureType_NORMALS);
        get_textures(ambient, aiTextureType_AMBIENT);
        get_textures(ambient_occlusion, aiTextureType_AMBIENT_OCCLUSION);
        get_textures(base_color, aiTextureType_BASE_COLOR);
        get_textures(height, aiTextureType_HEIGHT);
        get_textures(displacement, aiTextureType_DISPLACEMENT);
        get_textures(emissive, aiTextureType_EMISSIVE);
        get_textures(lightmap, aiTextureType_LIGHTMAP);

        // get mesh data
        for (size_t v_n = 0; v_n < msh->mNumVertices; v_n++) {
            auto vert = transform * msh->mVertices[v_n];
            _vertexes->push_back(vec3(vert.x, vert.y, vert.z));
        }
        for (size_t f_n = 0; f_n < msh->mNumFaces; f_n++) {
            auto fce = msh->mFaces[f_n];
            faces->push_back(make_tup<unsigned int, 3>({fce.mIndices[0], fce.mIndices[1], fce.mIndices[2]}));
        }
        for (unsigned int j = 0; j < msh->mNumVertices; ++j) {
            auto uv_coord = msh->mTextureCoords[0][j];// 0 = diffuse
            _diffuse_coordinates->push_back(vec3(uv_coord.x, uv_coord.y, uv_coord.z));
        }
        
        if (!meshes.data.contains(string(msh->mName.C_Str()))) {
            meshes.insert(new mesh(
                string(msh->mName.C_Str()),
                materials,
                _vertexes,
                _diffuse_coordinates,
                _vertex_normals,
                faces,
                _transform,
                diffuse_textures,
                specular_textures,
                normals_textures
            ));
        }
            
    }
}


mesh_dict mesh::from_file(string file_path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile( file_path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene) {
        std::stringstream ss;
        ss << "Failed to import model at \"" << file_path << "\"\nLOG:\n" << importer.GetErrorString() << "\n";
        throw std::runtime_error(ss.str());
    }
    mesh_dict meshes;
    
    process_node(scene->mRootNode, scene, meshes, scene->mRootNode->mTransformation, file_path);
    
    return meshes;
}

void mesh::create_VAO() {
    //VAO
    glGenVertexArrays(1, &this->gl_VAO);
    glBindVertexArray(this->gl_VAO);

    vector<GLfloat> gl_verts;
    vector<GLuint> gl_inds;
    this->get_gl_vert_inds(*this->vertexes, &gl_inds);
    this->get_gl_verts(*this->vertexes, &gl_verts);

    
    this->indicies_size = gl_inds.size();
    this->verticies_size = gl_verts.size();
    //VBO
    glGenBuffers(1, &this->gl_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, this->gl_VBO);
    glBufferData(GL_ARRAY_BUFFER, this->verticies_size * sizeof(GLfloat), &gl_verts[0], GL_DYNAMIC_DRAW);
    
    //EBO
    glGenBuffers(1, &this->gl_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->gl_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indicies_size * sizeof(GLuint), &gl_inds[0], GL_DYNAMIC_DRAW);

    // Vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    if (!this->faces->empty()) {
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
            (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void mesh::get_gl_verts(vector<vec3> vertexes, vector<float>* mut_verts) {
    vec3 vert;
    for (size_t i = 0; i < vertexes.size(); i++) {
        vert = vertexes[i];
        mut_verts->push_back(vert.axis.x);
        mut_verts->push_back(vert.axis.y);
        mut_verts->push_back(vert.axis.z);
        if (!this->diffuse_coordinates->empty()) {
            mut_verts->push_back((*this->diffuse_coordinates)[i].axis.x);
            mut_verts->push_back((*this->diffuse_coordinates)[i].axis.y);
        }
    }
}

void mesh::get_gl_vert_inds(vector<vec3> vertexes, vector<unsigned int>* mut_inds) {
    for (tup<unsigned int, 3> fce : *this->faces) {
        mut_inds->push_back(fce[0]);
        mut_inds->push_back(fce[1]);
        mut_inds->push_back(fce[2]);
    }
}