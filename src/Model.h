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
    string name;
    int id = -1;
    /*offset matrix transforms vertex from model space to bone space*/
    matrix4x4 offset = matrix4x4(1.0f);
};

class model {
public:
    model(){}
    model(RC<mesh_dict*>* mesh_data, bool animated);
    inline void render(object3d* obj, camera& camera, window* window) {
        render_meshdict(mesh_data, obj, camera, window);
    }

    ~model();
    RC<mesh_dict*>* mesh_data;
    bool use_default_material_properties = false;
    
    // ANIMATION STUFF
    bool animated = false;
    vector<bone_info> bone_info_list;
    int bone_counter = 0;
    std::map<string, animation*> animations;
    animator* animation_player = nullptr;
    object3d * owner = nullptr;
    //

    void play_animation(const string& animation);

    inline RC<model*>* from_file(string file_path, bool animated) {
        return mesh::from_file(file_path, animated);
    }

    inline static void set_vertex_bone_data_to_default(vertex* vert)
    {
        for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
        {
            vert->bone_ids[i] = -1;
            vert->weights[i] = 0.0f;
        }
    }

    inline static void set_vertex_bone_data(vertex* vert, int boneID, float weight)
    {
        for (int i = 0; i < MAX_BONE_INFLUENCE; ++i)
        {
            if (vert->bone_ids[i] < 0)
            {
                vert->weights[i] = weight;
                vert->bone_ids[i] = boneID;
                break;
            }
        }
    }

    inline bool in_bones(const string & name) {
        for (unsigned int i = 0; i < bone_info_list.size(); i++) {
            if (bone_info_list[i].name == name) {
                return true;
            }
        }
        return false;
    }

    inline bone_info* find_bone(const string & name) {
        // find the bone in the bone list
        for (unsigned int i = 0; i < bone_info_list.size(); i++) {
			if (bone_info_list[i].name == name) {
				return &bone_info_list[i];
			}
		}
		return nullptr;
    }

    inline void extract_bone_weight_for_vertices(vector<vertex>* vertices, aiMesh* mesh, const aiScene* scene)
    {
        // Set the max bones to 100
		unsigned int numBones = mesh->mNumBones > 100 ? 100 : mesh->mNumBones;
        int& bone_count_local = bone_counter;
		// For each bone
		for (unsigned int boneIndex = 0; boneIndex < numBones; ++boneIndex)
		{
			int boneID = -1;
			std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
			if (!in_bones(boneName)) {
				bone_info_list.push_back({ boneName, bone_count_local, AssimpGLMHelpers::ConvertMatrixToGLMFormat(&mesh->mBones[boneIndex]->mOffsetMatrix) });
				boneID = bone_count_local;
				bone_count_local++; 
			}
			else {
				for (unsigned int i = 0; i < bone_info_list.size(); i++) {
					if (bone_info_list[i].name == boneName) {
						boneID = bone_info_list[i].id;
						break;
					}
				}
			}
			if (boneID == -1) {
                throw std::runtime_error("Loading bones failed, invaild id (-1).");
            }

			// Get all vertex weights for current bone
			aiVertexWeight* weights = mesh->mBones[boneIndex]->mWeights;
			unsigned int numWeights = mesh->mBones[boneIndex]->mNumWeights;

			// For each weight at vertex x for current bone
			for (int weightIndex = 0; weightIndex < numWeights; ++weightIndex) {
				unsigned int vertexId = weights[weightIndex].mVertexId;
				float weight = weights[weightIndex].mWeight;
				if (vertexId > vertices->size())
                    throw std::runtime_error("Loading bones failed, vertex id exceeds ammount of vertices.");

				set_vertex_bone_data(&(*vertices)[vertexId], boneID, weight);
			}
		}
    }
private:
    void render_meshdict(RC<mesh_dict*>* _mesh_data, object3d* obj, camera& camera, window* window);
};

typedef RC<model*>* rc_model;