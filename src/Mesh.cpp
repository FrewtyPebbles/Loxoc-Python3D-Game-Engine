#include "Mesh.h"
#include "Vec3.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include "debug.h"
#include "util.h"
#include <filesystem>
#include <sstream>
#include "Model.h"
#include "Animation.h"


string fix_texture_path(string file_path, string file) {
    return std::filesystem::absolute(std::filesystem::path(str_tool::rem_file_from_path(file_path) + "/textures/" + str_tool::rem_path_from_file(file))).string();
}

void mesh::process_node(rc_model model, aiNode* node, const aiScene* scene, rc_mesh_dict last_mesh_dict, const aiMatrix4x4& transform, string file_path) {
    
    // itterate meshes for the node
    auto t_aivec3 = transform * aiVector3D(1.0f, 1.0f, 1.0f);
    for (size_t m_n = 0; m_n < node->mNumMeshes; m_n++) {
        auto msh = scene->mMeshes[node->mMeshes[m_n]];
        auto mesh_name = string(msh->mName.C_Str());
        rc_material mesh_material = new RC(new material(new RC(shader::from_file(get_mod_path() + (model->data->animated ? "/default_vertex_animated.glsl" : "/default_vertex.glsl"), ShaderType::VERTEX)), new RC(shader::from_file(get_mod_path() + "/default_fragment.glsl", ShaderType::FRAGMENT))));
        vector<tup<unsigned int, 3>>* faces = new vector<tup<unsigned int, 3>>();
        vector<vertex>* _vertexes = new vector<vertex>();

        vec3 _transform = vec3(t_aivec3.x, t_aivec3.y, t_aivec3.z);
        vec3 aabb_max = vec3(0,0,0);
        vec3 aabb_min = vec3(0,0,0);
        
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
            vertex push_vert;

            model::set_vertex_bone_data_to_default(&push_vert);

            auto vert = transform * msh->mVertices[v_n];
            push_vert.position = glm::vec3(vert.x, vert.y, vert.z); 
            auto new_vert_mag = vec3(vert.x, vert.y, vert.z).get_magnitude();
            if (radius < new_vert_mag) {
                radius = new_vert_mag;
            }
            // get max and min aabb
            if (vert.x > aabb_max.get_x())
                aabb_max.axis.x = vert.x;
            if (vert.y > aabb_max.get_y())
                aabb_max.axis.y = vert.y;
            if (vert.z > aabb_max.get_z())
                aabb_max.axis.z = vert.z;

            if (vert.x < aabb_min.get_x())
                aabb_min.axis.x = vert.x;
            if (vert.y < aabb_min.get_y())
                aabb_min.axis.y = vert.y;
            if (vert.z < aabb_min.get_z())
                aabb_min.axis.z = vert.z;

            auto uv_coord = msh->mTextureCoords[0][v_n];// 0 = diffuse
            push_vert.tex_coords = glm::vec3(uv_coord.x, uv_coord.y, uv_coord.z);

            if (has_norms) {
                auto norm_coord = msh->mNormals[v_n];// 0 = diffuse
                push_vert.normal = glm::vec3(norm_coord.x, norm_coord.y, norm_coord.z);
            } else {
                push_vert.normal = glm::vec3(0.0f, 0.0f, 0.0f);
            }

            _vertexes->push_back(push_vert);
        }

        for (size_t f_n = 0; f_n < msh->mNumFaces; f_n++) {
            auto fce = msh->mFaces[f_n];
            faces->push_back(make_tup<unsigned int, 3>({fce.mIndices[0], fce.mIndices[1], fce.mIndices[2]}));
        }

        if (mesh_material->data->diffuse_texture == nullptr) { // TODO add logic to allow for mesh color
            // insert default texture 
            mesh_material->data->diffuse_texture = new RC(new texture(get_mod_path() + "/MissingTexture.jpg", TextureWraping::REPEAT, TextureFiltering::LINEAR));
        }

        
        model->data->extract_bone_weight_for_vertices(_vertexes, msh, scene);
  
        for (auto& v : *_vertexes) {
            v.weights = glm::normalize(v.weights);
        }

        auto ret_mesh = new RC(new mesh(mesh_name, mesh_material, _vertexes, faces, _transform, model->data->animated));
        ret_mesh->data->radius = radius;
        ret_mesh->data->aabb_max = aabb_max;
        ret_mesh->data->aabb_min = aabb_min;
        last_mesh_dict->data->insert(ret_mesh);
    }
    
    // traverse to deeper nodes
    for (size_t c_n = 0; c_n < node->mNumChildren; c_n++) {
        auto child_mesh_dict = new RC(new mesh_dict());
        child_mesh_dict->data->name = node->mChildren[c_n]->mName.C_Str();
        process_node(model, node->mChildren[c_n], scene, child_mesh_dict, transform * node->mChildren[c_n]->mTransformation, file_path);
        
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


rc_model mesh::from_file(string file_path, bool animated) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile( file_path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene) {
        std::stringstream ss;
        ss << "Failed to import model at \"" << file_path << "\"\nLOG:\n" << importer.GetErrorString() << "\n";
        throw std::runtime_error(ss.str());
    }
    auto curren_mesh_dict = new RC(new mesh_dict());

    curren_mesh_dict->data->name = file_path;
    auto ret = new RC(new model( 
        curren_mesh_dict,
        animated
    ));
    
    ret->data->animated = scene->mNumAnimations > 0;

    process_node(ret, scene->mRootNode, scene, curren_mesh_dict, scene->mRootNode->mTransformation, file_path);

    for (int i = 0; i < scene->mNumAnimations; i++)  {
        ret->data->animations[scene->mAnimations[i]->mName.data] = new animation(scene, scene->mAnimations[i], ret);
        ret->data->animated = true;
    }
    
    return ret;
}
 
void mesh::create_VAO() {
    //VAO
    glGenVertexArrays(1, &this->gl_VAO);
    glBindVertexArray(this->gl_VAO);

    vector<GLuint> gl_inds;
    this->get_gl_vert_inds(&gl_inds);
    
    this->indicies_size = gl_inds.size();

    //VBO
    glGenBuffers(1, &this->gl_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, this->gl_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices->size() * sizeof(vertex), &(*vertices)[0], GL_STATIC_DRAW); 
    
    //EBO
    glGenBuffers(1, &this->gl_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->gl_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indicies_size * sizeof(GLuint), &gl_inds[0], GL_STATIC_DRAW);

    // Vertex attributes (verticies)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)0);
    glEnableVertexAttribArray(0);

    // Vertex attributes (normals)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, normal));
    glEnableVertexAttribArray(1);

    // if (!this->faces->empty()) {
        // Vertex attributes (texcoords)
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex),
            (void*)offsetof(vertex, tex_coords));
        glEnableVertexAttribArray(2);
    // }

    if (this->is_animated) {
        // Vertex attributes (bone_ids)
        glVertexAttribIPointer(3, 4, GL_INT, sizeof(vertex), (void*)offsetof(vertex, bone_ids));
        glEnableVertexAttribArray(3);
        // Vertex attributes (weights)
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, weights));
        glEnableVertexAttribArray(4); 
    }

    glBindVertexArray(0);
}

void mesh::get_gl_vert_inds(vector<unsigned int>* mut_inds) {
    for (tup<unsigned int, 3> fce : *this->faces) {
        mut_inds->push_back(fce[0]);
        mut_inds->push_back(fce[1]);
        mut_inds->push_back(fce[2]);
    }
}