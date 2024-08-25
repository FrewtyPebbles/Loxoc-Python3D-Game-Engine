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

using std::vector;
using std::string;

// Components of the bone

//// Keyframes

struct key_position {
    vec3 position;
    float time_stamp;
};

struct key_rotation {
    quaternion orientation;
    float time_stamp;
};

struct key_scale {
    vec3 scale;
    float time_stamp;
};

// The bone!

class bone {
public:
    string name;
    int id = -1;
    matrix4x4 local_transform = matrix4x4(1.0f); // This is the transform matrix that all of the lerp'd keyframes are baked into
    // it is the orientation of the bone in the animation at the current frame.
private:
    vector<key_position> positions;
    vector<key_rotation> rotations;
    vector<key_scale> scales;
    int positions_size, rotations_size, scales_size;
public:
    // constructors
    bone(const string& name, int id, const aiNodeAnim* channel):// aiNodeAnim is the animation data for a bone
        name(name),
        id(id),
        local_transform(1.0f)
    {

        // POSITION
        positions_size = channel->mNumPositionKeys;
        for (int pos_i = 0; pos_i < positions_size; ++pos_i) {
            aiVector3D ai_pos = channel->mPositionKeys[pos_i].mValue;
            float time_stamp = (float)channel->mPositionKeys[pos_i].mTime;
            key_position data = {vec3(ai_pos.x, ai_pos.y, ai_pos.z), time_stamp};
            positions.push_back(data);
        }

        // ROTATION
        rotations_size = channel->mNumRotationKeys;
        for (int rot_i = 0; rot_i < rotations_size; ++rot_i) {
            aiQuaternion ai_rot = channel->mRotationKeys[rot_i].mValue;
            float time_stamp = channel->mRotationKeys[rot_i].mTime;
            key_rotation data = {quaternion(ai_rot.w, ai_rot.x, ai_rot.y, ai_rot.z), time_stamp};
            rotations.push_back(data);
        }

        // SCALE
        scales_size = channel->mNumScalingKeys;
        for (int scale_i = 0; scale_i < scales_size; ++scale_i) {
            aiVector3D ai_scale = channel->mScalingKeys[scale_i].mValue;
            float time_stamp = channel->mScalingKeys[scale_i].mTime;
            key_scale data = {vec3(ai_scale.x, ai_scale.y, ai_scale.z), time_stamp};
            scales.push_back(data);
        }
    }
    // METHODS

    //// Helper Functions and Trunks

    inline void update(float animation_time) {
        // This function updates the whole bone transform.  Acts as a call trunk.
        matrix4x4 translation = interpolate_position(animation_time);
        matrix4x4 rotation = interpolate_rotation(animation_time);
        matrix4x4 scale = interpolate_scale(animation_time);
        local_transform = translation * rotation * scale;
    }

    inline int get_position_index(float animation_time) {
        // itterates all positions untill it finds a keyframe with a higher timestamp than the current time
        for (int i = 0; i < positions_size - 1; ++i) {
            if (animation_time < positions[i + 1].time_stamp)
                return i;
        }
        return positions_size-2;
    }

    inline int get_rotation_index(float animation_time) {
        // itterates all rotations untill it finds a keyframe with a higher timestamp than the current time
        for (int i = 0; i < rotations_size - 1; ++i) {
            if (animation_time < rotations[i + 1].time_stamp)
                return i;
        }
        return rotations_size-2;
    }

    inline int get_scale_index(float animation_time) {
        // itterates all scales untill it finds a keyframe with a higher timestamp than the current time
        for (int i = 0; i < scales_size - 1; ++i) {
            if (animation_time < scales[i + 1].time_stamp)
                return i;
        }
        return scales_size-2;
    }

private:
    // THE MEAT

    inline float get_scale_factor(float prev_time_stamp, float next_time_stamp, float animation_time) {
        // returns the time to reach the next frame as a ratio between 0.0 and 1.0
        float scale_factor = 0.0f;
        float mid_way_len = animation_time - prev_time_stamp;// this is how far it has traveled between prev and next
        float transition_len = next_time_stamp - prev_time_stamp;
        scale_factor = mid_way_len / transition_len;// creates a ratio ie: interpolates the distance to the next keyframe between 0.0 and 1.0
        return scale_factor;
    }

    inline matrix4x4 interpolate_position(float animation_time) {
        // interpolates the position between the prev and next keyframe position
        if (positions_size == 1)
            return matrix4x4(1.0f).translate(positions[0].position);
        
        int pos_0_i = get_position_index(animation_time);
        int pos_1_i = pos_0_i + 1;
        float scale_factor = get_scale_factor(positions[pos_0_i].time_stamp, positions[pos_1_i].time_stamp, animation_time);
        vec3 final_position = positions[pos_0_i].position.lerp(positions[pos_1_i].position, scale_factor);
        return matrix4x4(1.0f).translate(final_position);
    }

    inline matrix4x4 interpolate_rotation(float animation_time) {
        // interpolates the rotation between the prev and next keyframe rotation
        if (rotations_size == 1) {
            auto rot = rotations[0].orientation.get_normalized();
            return matrix4x4(rot);
        }

        int rot_0_i = get_rotation_index(animation_time);
        int rot_1_i = rot_0_i + 1;
        float scale_factor = get_scale_factor(rotations[rot_0_i].time_stamp, rotations[rot_1_i].time_stamp, animation_time);
        quaternion final_rot = rotations[rot_0_i].orientation.slerp(rotations[rot_1_i].orientation, scale_factor);
        final_rot = final_rot.get_normalized();
        return matrix4x4(final_rot);
    }

    inline matrix4x4 interpolate_scale(float animation_time) {
        // interpolates the scale between the prev and next keyframe scale
        if (scales_size == 1)
            return matrix4x4(1.0f).scale(scales[0].scale);

        int pos_0_i = get_scale_index(animation_time);
        int pos_1_i = pos_0_i + 1;
        float scale_factor = get_scale_factor(scales[pos_0_i].time_stamp, scales[pos_1_i].time_stamp, animation_time);
        vec3 final_scale = scales[pos_0_i].scale.lerp(scales[pos_1_i].scale, scale_factor);
        return matrix4x4(1.0f).scale(final_scale);
    }

};
