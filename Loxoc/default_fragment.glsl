#version 450 core


struct PointLight {
    vec3 position;
    vec3 color;
	float radius;
};

struct Material {
    vec3 ambient;
    sampler2D diffuse_map;
    sampler2D specular_map;
    float shine;
};

#define MAX_POINT_LIGHTS 25

uniform PointLight point_lights[MAX_POINT_LIGHTS];
uniform int total_point_lights;

uniform vec3 viewPos;
uniform vec3 ambient_light;

uniform Material material;


in vec3 FragPos;  
in vec3 Normal;
in vec2 TexCoord;

out vec4 FragColor;


vec4 LOXOC_default(vec4 base_color) {
	vec3 ambient = vec3(0.0);
    vec3 diffuse = vec3(0.0);
    vec3 specular = vec3(0.0);

    for(int i = 0; i < total_point_lights; i++) {
		PointLight current_l = point_lights[i];
		float distance_ratio = max(0.0, length(FragPos - current_l.position)/current_l.radius);

        // Ambient
        vec3 am_t = current_l.color * vec3(texture(material.diffuse_map, TexCoord));
        ambient += mix(am_t, vec3(0.0), distance_ratio);
        
        // Diffuse 
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(current_l.position - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 dif_t = current_l.color * (diff * vec3(texture(material.diffuse_map, TexCoord)));
        diffuse += mix(dif_t, vec3(0.0), distance_ratio);
        
        // Specular
        float specularStrength = 0.5;
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);  
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shine);
        vec3 sp_t = current_l.color * (spec * vec3(texture(material.specular_map, TexCoord)));
        specular += mix(sp_t, vec3(0.0), distance_ratio);
    }

    return vec4(max(ambient_light * base_color.xyz, ambient + diffuse + specular), 1.0);
}

void main() {
	
    vec4 texColor = texture(material.diffuse_map, TexCoord);
    vec4 default_render = LOXOC_default(texColor);
    FragColor = default_render;
}