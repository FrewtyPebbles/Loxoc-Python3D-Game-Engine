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

void mesh::process_node(aiNode* node, const aiScene* scene, rc_mesh_dict last_mesh_dict, const aiMatrix4x4& transform, string file_path) {
    
    // itterate meshes for the node
    auto t_aivec3 = transform * aiVector3D(1.0f, 1.0f, 1.0f);
    for (size_t m_n = 0; m_n < node->mNumMeshes; m_n++) {
        auto msh = scene->mMeshes[node->mMeshes[m_n]];
        auto mesh_name = string(msh->mName.C_Str());
        rc_material mesh_material = new RC(new material(new RC(shader::from_file(get_mod_path() + "/default_vertex.glsl", ShaderType::VERTEX)), new RC(shader::from_file(get_mod_path() + "/default_fragment.glsl", ShaderType::FRAGMENT))));
        vector<vec3>* _vertexes = new vector<vec3>();
        vector<vec3>* _diffuse_coordinates = new vector<vec3>();
        vector<vec3>* _vertex_normals = new vector<vec3>();
        vector<tup<unsigned int, 3>>* faces = new vector<tup<unsigned int, 3>>();
        vec3 _transform = vec3(t_aivec3.x, t_aivec3.y, t_aivec3.z);
        
        // get material data
        auto ai_mat = scene->mMaterials[msh->mMaterialIndex];

        aiString name;
        ai_mat->Get(AI_MATKEY_NAME, name);
        mesh_material->data->name = name.C_Str();

        aiColor3D diffuseColor(0.0f, 0.0f, 0.0f);
        ai_mat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);
        mesh_material->data->diffuse.set_x(diffuseColor.r);
        mesh_material->data->diffuse.set_y(diffuseColor.g);
        mesh_material->data->diffuse.set_z(diffuseColor.b);

        aiColor3D specularColor(0.0f, 0.0f, 0.0f);
        ai_mat->Get(AI_MATKEY_COLOR_SPECULAR, specularColor);
        mesh_material->data->specular.set_x(specularColor.r);
        mesh_material->data->specular.set_y(specularColor.g);
        mesh_material->data->specular.set_z(specularColor.b);

        aiColor3D ambientColor(0.0f, 0.0f, 0.0f);
        ai_mat->Get(AI_MATKEY_COLOR_AMBIENT, ambientColor);
        mesh_material->data->ambient.set_x(ambientColor.r);
        mesh_material->data->ambient.set_y(ambientColor.g);
        mesh_material->data->ambient.set_z(ambientColor.b);

        ai_mat->Get(AI_MATKEY_SHININESS, mesh_material->data->shine);
        mesh_material->data->shine = std::max(mesh_material->data->shine, 1.0f);

        // get texture data
        get_textures(diffuse, aiTextureType_DIFFUSE);
        get_textures(specular, aiTextureType_SPECULAR);
        get_textures(normals, aiTextureType_NORMALS);

        float radius = 0;
        bool has_norms = msh->HasNormals();
        // get mesh data
        for (size_t v_n = 0; v_n < msh->mNumVertices; v_n++) {
            auto vert = transform * msh->mVertices[v_n];
            _vertexes->push_back(vec3(vert.x, vert.y, vert.z));
            auto new_vert_mag = vec3(vert.x, vert.y, vert.z).get_magnitude();
            if (radius < new_vert_mag) {
                radius = new_vert_mag;
            }

            auto uv_coord = msh->mTextureCoords[0][v_n];// 0 = diffuse
            _diffuse_coordinates->push_back(vec3(uv_coord.x, uv_coord.y, uv_coord.z));

            if (has_norms) {
                auto norm_coord = msh->mNormals[v_n];// 0 = diffuse
                _vertex_normals->push_back(vec3(norm_coord.x, norm_coord.y, norm_coord.z));
            } else {
                _vertex_normals->push_back(vec3(0.0f, 0.0f, 0.0f));
            }
        }

        for (size_t f_n = 0; f_n < msh->mNumFaces; f_n++) {
            auto fce = msh->mFaces[f_n];
            faces->push_back(make_tup<unsigned int, 3>({fce.mIndices[0], fce.mIndices[1], fce.mIndices[2]}));
        }

        if (mesh_material->data->diffuse_texture == nullptr) {
            // insert default texture
            mesh_material->data->diffuse_texture = new RC(new texture(get_mod_path() + "/MissingTexture.jpg", TextureWraping::REPEAT, TextureFiltering::LINEAR));
        }
        auto ret_mesh = new RC(new mesh(
            mesh_name,
            mesh_material,
            _vertexes,
            _diffuse_coordinates,
            _vertex_normals,
            faces,
            _transform
        ));
        ret_mesh->data->radius = radius;
        last_mesh_dict->data->insert(ret_mesh);
    }
    
    // traverse to deeper nodes
    for (size_t c_n = 0; c_n < node->mNumChildren; c_n++) {
        auto child_mesh_dict = new RC(new mesh_dict());
        child_mesh_dict->data->name = node->mChildren[c_n]->mName.C_Str();
        process_node(node->mChildren[c_n], scene, child_mesh_dict, transform * node->mChildren[c_n]->mTransformation, file_path);
        
        if (child_mesh_dict->data->data.size() == 1 && // Check if it is duplicating the name with the dict
                    child_mesh_dict->data->data.contains(child_mesh_dict->data->name)) {
            last_mesh_dict->data->insert(child_mesh_dict->data->data[child_mesh_dict->data->name]);
            delete child_mesh_dict->data;
            delete child_mesh_dict;
        }    
        else
            last_mesh_dict->data->insert(child_mesh_dict);
    }
}


rc_mesh_dict mesh::from_file(string file_path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile( file_path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene) {
        std::stringstream ss;
        ss << "Failed to import model at \"" << file_path << "\"\nLOG:\n" << importer.GetErrorString() << "\n";
        throw std::runtime_error(ss.str());
    }
    auto curren_mesh_dict = new RC(new mesh_dict());

    curren_mesh_dict->data->name = file_path;
    
    process_node(scene->mRootNode, scene, curren_mesh_dict, scene->mRootNode->mTransformation, file_path);
    
    return curren_mesh_dict;
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
    glBufferData(GL_ARRAY_BUFFER, this->verticies_size * sizeof(GLfloat), &gl_verts[0], GL_STATIC_DRAW);
    
    //EBO
    glGenBuffers(1, &this->gl_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->gl_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indicies_size * sizeof(GLuint), &gl_inds[0], GL_STATIC_DRAW);

    // Vertex attributes (verticies)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Vertex attributes (normals)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    if (!this->faces->empty()) {
        // Vertex attributes (texcoords)
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
            (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
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
        mut_verts->push_back((*this->vertex_normals)[i].axis.x);
        mut_verts->push_back((*this->vertex_normals)[i].axis.y);
        mut_verts->push_back((*this->vertex_normals)[i].axis.z);
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