#version 330 core

uniform sampler2D sprite;

in vec2 TexCoord;

out vec4 FragColor;

void main() {
	vec4 tex_color = texture(sprite, TexCoord);
	FragColor = tex_color;
}