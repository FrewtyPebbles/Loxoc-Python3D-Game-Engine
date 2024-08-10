#version 450 core

in vec2 tex_coords;
in vec4 particle_color;
in float life;

out vec4 color;

uniform sampler2D sprite;

void main()
{
    vec3 pcol = normalize(particle_color.rgb);
    color = texture(sprite, tex_coords) * vec4(pcol, particle_color.a);
    if (color.a < 0.01)
        discard;
}