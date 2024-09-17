#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

uniform mat4 transform2D;

uniform mat4 projection;

out vec2 TexCoord;

void main() {
	gl_Position = projection * transform2D * vec4(aPos, 1.0f);
	TexCoord = aTexCoord;
}
