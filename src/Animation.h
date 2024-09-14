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
#include "Material.h"

using std::vector;
using std::string;
using std::map;

// Tree of assimp data to help decouple our animations from assimp
struct assimp_node_data {
    matrix4x4 transformation = matrix4x4(1.0f);
    string name = "";
    int children_size = 0;
    vector<assimp_node_data> children;
};

// ANIMATION CLASS (animator class is below this)

class animation {
public:
    float duration = 0.0f;
    float ticks_per_second = 0.0f;
    vector<bone> bones;
    vector<bone_info> bone_info_list;
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
        duration = (float)animation->mDuration;
        ticks_per_second = (float)animation->mTicksPerSecond;


        read_heirarchy_data(&assimp_animation_tree, scene->mRootNode);
        // assimp_animation_tree.transformation = matrix4x4(1.0f);
        read_missing_bones(animation, model);
        dbg_vis_init();
    }

    animation(const aiScene* scene, const aiAnimation * animation, RC<model*>* model) {
        // load the animation in from an aiScene and aiAnimation

        duration = (float)animation->mDuration;
        ticks_per_second = (float)animation->mTicksPerSecond;

        read_heirarchy_data(&assimp_animation_tree, scene->mRootNode);
        // assimp_animation_tree.transformation = matrix4x4(1.0f);
        read_missing_bones(animation, model);
        dbg_vis_init();
    }
    
    ~animation(){}

    // METHODS

    inline bone* find_bone(const string & name) {
        // find the bone in the bone list
        for (unsigned int i = 0; i < bones.size(); i++) {
			if (bones[i].name == name) {
				return &bones[i];
			}
		}
		return nullptr;
    }

    inline bone* find_bone_id(int id) {
        // find the bone in the bone list
        auto iter = std::find_if(bones.begin(), bones.end(), 
            [&](const bone& bone) {
                return bone.id == id;
            }
        );
        if (iter == bones.end()) return nullptr;
        else return &(*iter);
    }

    inline const assimp_node_data* get_assimp_animation_tree() {return &assimp_animation_tree;}

private:
    inline void read_missing_bones(const aiAnimation* animation, RC<model*>* model) {
        // sets the bone info map for the animation.

        auto& bone_info_list = model->data->bone_info_list;
        int& bone_counter = model->data->bone_counter;

        //read through all channels to get bone info
        for (int i = 0; i < animation->mNumChannels; i++) {
            auto channel = animation->mChannels[i];
            string bone_name = channel->mNodeName.data;

            auto b = model->data->find_bone(bone_name);
            if (!b) {
                bone_info b_i_new;
                b_i_new.name = bone_name;
                b_i_new.id = bone_counter;
                bone_counter++;
                model->data->bone_info_list.push_back(b_i_new);
			}
            b = model->data->find_bone(bone_name);

            // add the bone of this current channel to our bone pile.
            bones.push_back(bone(bone_name, b->id, channel));
        }

        // set the new bone info list.
        this->bone_info_list = bone_info_list;
    }

    inline void read_heirarchy_data(assimp_node_data * parent, const aiNode* src) {
        if (!src)
            throw std::runtime_error("Failed to load Assimp animation tree data.");

        parent->name = src->mName.data;
        parent->transformation = matrix4x4(AssimpGLMHelpers::ConvertMatrixToGLMFormat(&src->mTransformation));
        parent->children_size = src->mNumChildren;

        for (int i = 0; i < src->mNumChildren; i++) {
            assimp_node_data new_data;
            read_heirarchy_data(&new_data, src->mChildren[i]);
            parent->children.push_back(new_data);
        }
    }

public:
    //// debug
    inline void dbg_vis_init() {
        unsigned int vertexShader, geometryShader, fragmentShader;
        // Vertex Shader source code
        const char* vertexShaderSource = R"(
            #version 450 core
            layout(location = 0) in vec3 aPos;

            uniform mat4 transform;

            void main()
            {
                gl_Position = transform * vec4(aPos, 1.0);
            }
        )";

        // Geometry Shader source code
        const char* geometryShaderSource = R"(
            #version 450 core
            layout(lines) in;
            layout(line_strip, max_vertices = 4) out;

            void main()
            {
                vec4 midpoint = (gl_in[0].gl_Position + gl_in[1].gl_Position) / 2.0;

                // Emit original line
                gl_Position = gl_in[0].gl_Position;
                EmitVertex();
                
                gl_Position = gl_in[1].gl_Position;
                EmitVertex();

                EndPrimitive();

                // Emit a new line from midpoint to a shifted point
                gl_Position = midpoint;
                EmitVertex();
                
                gl_Position = midpoint + vec4(0.1, 0.1, 0.0, 0.0);  // Slightly offset the second point
                EmitVertex();

                EndPrimitive();
            }
        )";

        // Fragment Shader source code
        const char* fragmentShaderSource = R"(
            #version 450 core
            out vec4 FragColor;

            void main()
            {
                FragColor = vec4(0.0, 1.0, 0.0, 1.0); // green color
            }
        )";
        
        // Vertex Shader
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
        glCompileShader(vertexShader);

        // Geometry Shader
        geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometryShader, 1, &geometryShaderSource, nullptr);
        glCompileShader(geometryShader);
        
        // Fragment Shader
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
        glCompileShader(fragmentShader);
        
        // Shader Program
        debug_shader = glCreateProgram();
        glAttachShader(debug_shader, vertexShader);
        glAttachShader(debug_shader, geometryShader);
        glAttachShader(debug_shader, fragmentShader);
        glLinkProgram(debug_shader);

        // these errors should never be thrown.
        GLint success;
        GLchar infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cerr << "ERROR: Vertex Shader Compilation Failed\n" << infoLog << std::endl;
        }
        
        glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(geometryShader, 512, NULL, infoLog);
            std::cerr << "ERROR: Geometry Shader Compilation Failed\n" << infoLog << std::endl;
        }
        
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            std::cerr << "ERROR: Fragment Shader Compilation Failed\n" << infoLog << std::endl;
        }
        
        glGetProgramiv(debug_shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(debug_shader, 512, NULL, infoLog);
            std::cerr << "ERROR: Shader Program Linking Failed\n" << infoLog << std::endl;
        }
        
        // Clean up shaders
        glDeleteShader(vertexShader);
        glDeleteShader(geometryShader);
        glDeleteShader(fragmentShader);
        
        // Generate and bind VAO
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        // Generate and bind VBO for vertices
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, (bones.size()-1)*2 * sizeof(glm::vec3), nullptr, GL_DYNAMIC_DRAW);
        // Set vertex attribute pointers
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }

    inline void dbg_push_bone(matrix4x4 mat) {
        debug_bones.push_back(mat.mat * glm::vec4(1.0f));
    }
    inline void dbg_render(const camera * cam, const matrix4x4 & model_mat) {
        glUseProgram(this->debug_shader);

        auto t_loc = glGetUniformLocation(debug_shader, "transform");
        glUniformMatrix4fv(t_loc, 1, GL_FALSE, glm::value_ptr(cam->projection.mat * cam->view.mat * model_mat.mat));

        // update the bones buffer
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, debug_bones.size() * sizeof(glm::vec3), debug_bones.data());
        
        glBindVertexArray(VAO);
        glDrawArrays(GL_LINES, 0, debug_bones.size());
        glBindVertexArray(0);
        debug_bones.clear();
    }

private:
    //// debug
    vector<glm::vec3> debug_bones; // this is set in the animator.
    unsigned int VAO = 0, VBO = 0, debug_shader = 0;
    //
};

// ANIMATOR CLASS

class animator {
public:
    // attributes
    vector<matrix4x4> final_bone_matricies;
    animation* current_animation = nullptr;
    float current_time = 0.0f;
    float delta_time = 0.0f;
    bool show_debug = false;

    inline void render_debug(const camera * cam, const matrix4x4 & model_mat) {
        if (show_debug && current_animation)
            current_animation->dbg_render(cam, model_mat);
    }

    // CONSTRUCTORS

    animator(animation* animation = nullptr) {
        current_time = 0.0f;
        current_animation = animation;
        final_bone_matricies.reserve(100);

        for (int i = 0; i < 100; i++)
            final_bone_matricies.push_back(matrix4x4(1.0f));
    }

    // METHODS

    inline void update(float dt) {
        // updates the current time of the animation and calls to calculate bone transforms
        delta_time = dt;
        if (current_animation) {
            current_time = fmod(current_time + current_animation->ticks_per_second * dt, current_animation->duration);
            // ensure that the current time is not exceeding the animation durration. mod so it loops back over.
            calculate_bone_transform(current_animation->get_assimp_animation_tree(), matrix4x4(1.0f));
        }
    }

    inline void set_uniforms(rc_material mater) {
        for (int i = 0; i < final_bone_matricies.size(); i++) {
            mater->data->set_uniform("final_bones_matrices[" + std::to_string(i) + "]", final_bone_matricies[i]);
        }
    }

    inline void play(animation* animation) {
        // plays the provided animation from the beginning
        current_animation = animation;
        current_time = 0.0f;
    }

    inline void calculate_bone_transform(const assimp_node_data* node, matrix4x4 parent_transform) {
        string node_name = node->name;
        matrix4x4 bone_transform = node->transformation;

        bone* bone = current_animation->find_bone(node_name);

        if (bone) {
            bone->update(current_time);
            bone_transform = bone->local_transform;
        }

        // transform local bone space to the parent bone space to follow bone space heirarchy
        matrix4x4 global_transformation = parent_transform * bone_transform;
        // 
        auto bone_info_list = current_animation->bone_info_list;
        for (unsigned int i = 0; i < bone_info_list.size(); i++) {
			if (bone_info_list[i].name == node_name) {
                int index = bone_info_list[i].id;
				matrix4x4 offset = bone_info_list[i].offset;
				final_bone_matricies[index] = global_transformation * offset;
				break;
			}
		}

        for (int i = 0; i < node->children_size; i++) {
            calculate_bone_transform(&node->children[i], global_transformation);
        }
    }
};
