#version 450 core

struct PointLight {
    vec3 position;
    vec3 color;
	float radius;
};

#define MAX_POINT_LIGHTS 25

uniform PointLight point_lights[MAX_POINT_LIGHTS];
uniform int total_point_lights;

uniform vec3 viewPos;
uniform vec3 ambient_light;

uniform sampler2D diffuse_map;
uniform sampler2D normal_map;
uniform sampler2D specular_map;
uniform sampler2D emissive_map;

in vec3 FragPos;  
in vec3 Normal;
in vec2 TexCoord;

out vec4 FragColor;

vec4 LOXOC_get_lighting(vec4 base_col) {
	vec3 ambient = vec3(0.0);
    vec3 diffuse = vec3(0.0);
    vec3 specular = vec3(0.0);

    for(int i = 0; i < total_point_lights; i++) {
		PointLight current_l = point_lights[i];
		float distance_ratio = max(0.0, length(FragPos - current_l.position)/current_l.radius);

        // Ambient
        float ambientStrength = 0.1;
        vec3 am_t = ambientStrength * current_l.color;
        ambient += mix(am_t, vec3(0.0), distance_ratio);
        
        // Diffuse 
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(current_l.position - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 dif_t = diff * current_l.color;
        diffuse += mix(dif_t, vec3(0.0), distance_ratio);
        
        // Specular
        float specularStrength = 0.5;
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);  
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        vec3 sp_t = specularStrength * spec * current_l.color;
        specular += mix(sp_t, vec3(0.0), distance_ratio);
    }

    return vec4(max(ambient_light, ambient + diffuse + specular), 1.0);
}

void main() {
	
    vec4 texColor = texture(diffuse_map, TexCoord);
    vec4 lighting = LOXOC_get_lighting(texColor);
    FragColor = lighting * texColor;
}