#pragma once
#include "Vec2.h"
#include "Vec3.h"
#include "Vec4.h"
#include "Matrix.h"
#include "Quaternion.h"
#include <vector>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "util.h"
#include "Model.h"
#include "Bone.h"
#include <map>
#include "RC.h"

using std::vector;
using std::string;
using std::map;

// Tree of assimp data to help decouple our animations from assimp
struct assimp_node_data {
    matrix4x4 transformation;
    string name;
    int children_size;
    vector<assimp_node_data> children;
};

// ANIMATION CLASS (animator class is below this)

class animation {
public:
    float duration;
    int ticks_per_second;
    vector<bone> bones;
    map<string, bone_info> bone_info_map;
private:
    assimp_node_data assimp_animation_tree;
public:
    // CONSTRUCTORS and destructor
    animation() = default;
    animation(const string & animation_path, RC<model*>* model) {
        // load the animation in from a file
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(animation_path, aiProcess_Triangulate);
        if (!(scene && scene->mRootNode))
            throw std::runtime_error("Failed to load animation in file: " + animation_path);

        auto animation = scene->mAnimations[0];
        duration = animation->mDuration;
        ticks_per_second = animation->mTicksPerSecond;
        read_heirarchy_data(assimp_animation_tree, scene->mRootNode);
        read_missing_bones(animation, *model->data);

    }

    animation(const aiScene* scene, const aiAnimation * animation, RC<model*>* model) {
        // load the animation in from an aiScene and aiAnimation

        duration = animation->mDuration;
        ticks_per_second = animation->mTicksPerSecond;
        read_heirarchy_data(assimp_animation_tree, scene->mRootNode);
        read_missing_bones(animation, *model->data);

    }
    
    ~animation(){}

    // METHODS

    inline bone* find_bone(const string & name) {
        // find the bone in the bone list
        auto iter = std::find_if(bones.begin(), bones.end(), 
            [&](const bone bone) {
                return bone.name == name;
            }
        );
        if (iter == bones.end()) return nullptr;
        else return &(*iter);
    }

    inline const assimp_node_data& get_assimp_animation_tree() {return assimp_animation_tree;}

private:
    inline void read_missing_bones(const aiAnimation* animation, model& model) {
        // sets the bone info map for the animation.
        int size = animation->mNumChannels;

        auto& new_bone_info_map = model.bone_info_map;
        int& bone_count = model.bone_counter;

        //read through all channels to get bone info
        for (int i = 0; i < size; i++) {
            auto channel = animation->mChannels[i];
            string bone_name = channel->mNodeName.data;

            // check if the bone in the assimp animation is in our bone info map
            if (new_bone_info_map.find(bone_name) == new_bone_info_map.end()) {
                // since it is not add its bone info or "register" the bone
                new_bone_info_map[bone_name].id = bone_count;
                bone_count++;
            }
            // add the bone of this current channel to our bone pile.
            bones.push_back(bone(channel->mNodeName.data, new_bone_info_map[channel->mNodeName.data].id, channel));
        }

        // set the new bone info map.
        bone_info_map = new_bone_info_map;
    }

    inline void read_heirarchy_data(assimp_node_data & dest, const aiNode* src) {
        if (!src)
            throw std::runtime_error("Failed to load Assimp animation tree data.");

        dest.name = src->mName.data;
        dest.transformation = AssimpGLMHelpers::ConvertMatrixToGLMFormat(src->mTransformation);
        dest.children_size = src->mNumChildren;

        for (int i = 0; i < src->mNumChildren; i++) {
            assimp_node_data new_data;
            read_heirarchy_data(new_data, src->mChildren[i]);
            dest.children.push_back(new_data);
        }
    }
};

// ANIMATOR CLASS

class animator {
public:
    // attributes
    vector<matrix4x4> final_bone_matricies;
    animation* current_animation;
    float current_time;
    float delta_time;

    // CONSTRUCTORS

    animator(animation* animation) {
        current_time = 0.0f;
        current_animation = animation;
        final_bone_matricies.reserve(100);

        for (int i = 0; i < 100; i++)
            final_bone_matricies.push_back(matrix4x4(1.0f));
    }

    // METHODS

    inline void update(float dt, GLuint shader_program) {
        // updates the current time of the animation and calls to calculate bone transforms
        delta_time = dt;
        if (current_animation) {
            current_time += current_animation->ticks_per_second * dt;
            // ensure that the current time is not exceeding the animation durration. mod so it loops back over.
            current_time = fmod(current_time, current_animation->duration);
            calculate_bone_transform(&current_animation->get_assimp_animation_tree(), matrix4x4(1.0f));
        }
        for (int i = 0; i < final_bone_matricies.size(); i++) {
            int loc = glGetUniformLocation(shader_program, ("final_bones_matrices[" + std::to_string(i) + "]").c_str());
            glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(final_bone_matricies[i].mat));
        }
    }

    inline void play(animation* animation) {
        // plays the provided animation from the beginning
        current_animation = animation;
        current_time = 0.0f;
    }

    inline void calculate_bone_transform(const assimp_node_data* node, matrix4x4 parent_transform) {
        string node_name = node->name;
        matrix4x4 node_transform = node->transformation;

        bone* bone = current_animation->find_bone(node_name);

        if (bone) {
            bone->update(current_time);
            node_transform = bone->local_transform;
        }

        // transform local bone space to the parent bone space to follow bone space heirarchy
        matrix4x4 global_transformation = parent_transform * node_transform;

        // 
        auto bone_info_map = current_animation->bone_info_map;
        if (bone_info_map.find(node_name) != bone_info_map.end()) {
            int index = bone_info_map[node_name].id;
            matrix4x4 offset = bone_info_map[node_name].offset;
            final_bone_matricies[index] = global_transformation * offset;
        }

        for (int i = 0; i < node->children_size; i++)
            calculate_bone_transform(&node->children[i], global_transformation);
    }
};
