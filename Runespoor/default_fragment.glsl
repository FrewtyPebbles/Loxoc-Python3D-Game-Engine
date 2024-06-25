#version 330 core

uniform sampler2D diffuse_map;//diffuse map
uniform sampler2D normal_map;//normal map
uniform sampler2D specular_map;//specular map
uniform sampler2D emissive_map;//emissive map

in vec2 TexCoord;

out vec4 FragColor;

void main() {
	FragColor = texture(diffuse_map, TexCoord);
}