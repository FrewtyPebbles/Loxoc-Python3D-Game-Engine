#pragma once
#include "RC.h"
#include "Mesh.h"
#include "Matrix.h"
#include "Camera.h"
#include "Window.h"
#include "Object3d.h"
#include "util.h"

class animation;
class animator;

// This class handles metadata, animations, meshes, etc of imported 3D assets.

struct bone_info
{
    /*id is index in finalBoneMatrices*/
    int id;
    /*offset matrix transforms vertex from model space to bone space*/
    matrix4x4 offset;
};

class model {
public:
    model(){}
    model(RC<mesh_dict*>* mesh_data, bool animated);
    inline void render(object3d* obj, camera& camera, window* window) {
        render_meshdict(mesh_data, obj, camera, window);
    }

    ~model() {
        delete animation_player;
        for (auto [k, v] : animations)
            delete v;
    }
    RC<mesh_dict*>* mesh_data;
    
    // ANIMATION STUFF
    bool animated = false;
    std::map<string, bone_info> bone_info_map;
    int bone_counter = 0;
    std::map<string, animation*> animations;
    animator* animation_player;
    //

    void play_animation(const string& animation);

    inline RC<model*>* from_file(string file_path, bool animated) {
        return mesh::from_file(file_path, animated);
    }

    inline static void set_vertex_bone_data_to_default(vertex& vert)
    {
        for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
        {
            vert.bone_ids[i] = -1;
            vert.weights[i] = 0.0f;
        }
    }

    inline static void set_vertex_bone_data(vertex& vert, int boneID, float weight)
    {
        for (int i = 0; i < MAX_BONE_INFLUENCE; ++i)
        {
            if (vert.bone_ids[i] < 0)
            {
                vert.weights[i] = weight;
                vert.bone_ids[i] = boneID;
                break;
            }
        }
    }

    inline void extract_bone_weight_for_vertices(vector<vertex>& vertices, aiMesh* mesh, const aiScene* scene)
    {
        for (int boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex)
        {
            int boneID = -1;
            std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
            if (bone_info_map.find(boneName) == bone_info_map.end())
            {
                bone_info newBoneInfo;
                newBoneInfo.id = bone_counter;
                newBoneInfo.offset = AssimpGLMHelpers::ConvertMatrixToGLMFormat(
                    mesh->mBones[boneIndex]->mOffsetMatrix);
                bone_info_map[boneName] = newBoneInfo;
                boneID = bone_counter;
                bone_counter++;
            }
            else
            {
                boneID = bone_info_map[boneName].id;
            }
            assert(boneID != -1);
            auto weights = mesh->mBones[boneIndex]->mWeights;
            int numWeights = mesh->mBones[boneIndex]->mNumWeights;

            for (int weightIndex = 0; weightIndex < numWeights; ++weightIndex)
            {
                int vertexId = weights[weightIndex].mVertexId;
                float weight = weights[weightIndex].mWeight;
                assert(vertexId <= vertices.size());
                set_vertex_bone_data(vertices[vertexId], boneID, weight);
            }
        }
    }
private:
    void render_meshdict(RC<mesh_dict*>* _mesh_data, object3d* obj, camera& camera, window* window);
};

typedef RC<model*>* rc_model;