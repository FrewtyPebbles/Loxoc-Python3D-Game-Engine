#version 330 core

uniform int focal_length;
uniform sampler2D ourTexture;

in float depth;
in vec2 TexCoord;

out vec4 FragColor;

void main() {
	FragColor = texture(ourTexture, TexCoord) - depth/focal_length;
}