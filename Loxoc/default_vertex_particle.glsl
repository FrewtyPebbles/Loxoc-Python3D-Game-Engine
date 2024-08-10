#version 450 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aOffset;
layout (location = 3) in vec4 aColor;
layout (location = 4) in vec2 aScale;
layout (location = 5) in float aLife;

out vec2 tex_coords;
out vec4 particle_color;
out float life;

uniform mat4 projection;
uniform mat4 view;

void main() {
    // Extract the right and up vectors from the view matrix and normalize them
    vec3 right = normalize(vec3(view[0][0], view[1][0], view[2][0]));
    vec3 up = normalize(vec3(view[0][1], view[1][1], view[2][1]));

    vec3 scaled_position = vec3(aPosition.xy * aScale.x, 0.0);

    // Reconstruct the billboarded vertex position using the right and up vectors
    vec3 bilboarded_position = aOffset + right * scaled_position.x + up * scaled_position.y;

    // Transform the position to clip space
    gl_Position = projection * view * vec4(bilboarded_position, 1.0);

    // Pass texture coordinates and color to the fragment shader
    tex_coords = aTexCoords;
    particle_color = aColor;
    life = aLife;
}