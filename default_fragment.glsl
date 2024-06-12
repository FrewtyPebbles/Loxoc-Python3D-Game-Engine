#version 330 core

uniform int focal_length;

in float depth;
out vec3 color;

void main() {
	color = vec3(1-1*abs(depth)/focal_length, 1-1*abs(depth)/focal_length, 1-1*abs(depth)/focal_length);
}