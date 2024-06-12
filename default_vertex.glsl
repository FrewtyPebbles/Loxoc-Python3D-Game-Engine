#version 330 core

layout (location = 0) in vec3 aPos;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


out float depth;

void main() {
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	depth = gl_Position.z;
}