#version 400 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in VS_OUT {
    vec3 position;
    vec4 particle_color;
    vec2 scale;
    float life;
    float starting_life;
} g_in[];

out vec2 tex_coords;
out vec4 particle_color;
out float life;
out float starting_life;

uniform mat4 projection;
uniform mat4 view;

void main() {
    // Get the camera's right and up vectors from the view matrix
    vec3 _right = normalize(vec3(view[0][0], view[1][0], view[2][0]));
    vec3 _up = normalize(vec3(view[0][1], view[1][1], view[2][1]));

    for (int i = 0; i < g_in.length(); ++i) {

        vec3 right = _right * g_in[i].scale.x;
        vec3 up = _up * g_in[i].scale.x;

        // Center position of the point (the center of the quad)
        vec4 center = gl_in[0].gl_Position;

        // Define the four corners of the quad
        vec4 corners[4];
        corners[0] = center + vec4(-right + up, 0.0);
        corners[1] = center + vec4(right + up, 0.0);
        corners[2] = center + vec4(-right - up, 0.0);
        corners[3] = center + vec4(right - up, 0.0);

        // Define the corresponding texture coordinates for each corner
        vec2 texCoords[4] = vec2[](vec2(0.0, 1.0), vec2(1.0, 1.0), vec2(0.0, 0.0), vec2(1.0, 0.0));



        for (int j = 0; j < 4; ++j) {
            tex_coords = texCoords[j];
            gl_Position = projection * view * corners[j];
            particle_color = g_in[i].particle_color;
            life = g_in[i].life;
            starting_life = g_in[i].starting_life;
            EmitVertex();
        }

        EndPrimitive();
    }
}
