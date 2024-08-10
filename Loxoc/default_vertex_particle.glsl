#version 450 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aScale;
layout (location = 3) in float aLife;


out VS_OUT {
    vec3 position;
    vec4 particle_color;
    vec2 scale;
    float life;
} vs_out;

void main() {
    vs_out.position = aPosition;
    vs_out.particle_color = aColor;
    vs_out.scale = aScale;
    vs_out.life = aLife;

    gl_Position = vec4(aPosition, 1.0);
}