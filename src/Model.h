#pragma once
#include "RC.h"
#include "Mesh.h"
#include "Matrix.h"
#include "Camera.h"
#include "Window.h"
#include "Object3d.h"

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
    model(RC<mesh_dict*>* mesh_data, bool animated) : mesh_data(mesh_data), animated(animated) {}
    void render_meshdict(RC<mesh_dict*>* _mesh_data, object3d* obj, camera& camera, window* window);
    RC<mesh_dict*>* mesh_data;
    bool animated = false;

private:
    std::map<string, bone_info> m_BoneInfoMap; //
    int m_BoneCounter = 0;
};