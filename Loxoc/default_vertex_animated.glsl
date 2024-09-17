#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout(location = 3) in ivec4 aBoneIds; 
layout(location = 4) in vec4 aWeights;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 final_bones_matrices[MAX_BONES];

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;

void main() {
    vec4 totalPosition = vec4(0.0f);
    vec3 totalNormal = vec3(0.0f);
    
    for(int i = 0 ; i < MAX_BONE_INFLUENCE ; i++) {
        if(aBoneIds[i] == -1)
            continue;
        if(aBoneIds[i] >= MAX_BONES) {
            totalPosition = vec4(aPos, 1.0f);
            totalNormal = aNormal;
            break;
        }
        
        vec4 localPosition = final_bones_matrices[aBoneIds[i]] * vec4(aPos, 1.0f);
        totalPosition += localPosition * aWeights[i];
        
        vec3 localNormal = mat3(final_bones_matrices[aBoneIds[i]]) * aNormal;
        totalNormal += localNormal * aWeights[i];
    }

    mat4 viewModel = view * model;
    // Final vertex position
    gl_Position = projection * viewModel * totalPosition;
    
    // Pass fragment position to the fragment shader
    FragPos = vec3(model * vec4(vec3(totalPosition), 1.0));
    
    // Corrected normal calculation
    Normal = totalNormal;
    
    // Pass texture coordinates
    TexCoord = aTexCoord;
}
