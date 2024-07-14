#version 450 core


struct PointLight {
    vec3 position;
    vec3 color;
	float radius;
    float constant;
    float linear;
    float quadratic;
    float intensity;
};

struct DirectionalLight {
    mat4 rotation;
    vec3 color;
	vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float intensity;
};

struct Material {
    vec3 ambient;
    sampler2D diffuse_map;
    sampler2D specular_map;
    float shine;
};

#define MAX_LIGHTS 50

uniform PointLight point_lights[MAX_LIGHTS];
uniform int total_point_lights;

uniform DirectionalLight directional_lights[MAX_LIGHTS];
uniform int total_directional_lights;

uniform vec3 viewPos;
uniform vec3 ambient_light;

uniform Material material;


in vec3 FragPos;  
in vec3 Normal;
in vec2 TexCoord;

out vec4 FragColor;

vec3 extractDirection(mat4 matrix) {
    return normalize(vec3(matrix[0][2], matrix[1][2], matrix[2][2]));
}

vec4 LOXOC_default(vec4 base_color) {
	vec3 ambient = vec3(0.0);
    vec3 diffuse = vec3(0.0);
    vec3 specular = vec3(0.0);

    for(int i = 0; i < total_directional_lights; i++) {
        DirectionalLight current_l = directional_lights[i];
        
        // Ambient
        vec3 am_t = current_l.color * vec3(texture(material.diffuse_map, TexCoord));
        ambient += am_t;
        
        // Diffuse 
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(-extractDirection(current_l.rotation));
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 dif_t = current_l.color * (diff * vec3(texture(material.diffuse_map, TexCoord)));
        diffuse += dif_t;
        
        // Specular
        float specularStrength = 0.5;
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);  
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shine);
        vec3 sp_t = current_l.color * (spec * vec3(texture(material.specular_map, TexCoord)));
        specular += sp_t;
    }

    for(int i = 0; i < total_point_lights; i++) {
		PointLight current_l = point_lights[i];
		float l_distance = length(current_l.position - FragPos);
        float attenuation = 1.0 / (current_l.constant + current_l.linear * l_distance + current_l.quadratic * (l_distance * l_distance));
        vec3 color = current_l.color * current_l.intensity;

        // Ambient
        vec3 am_t = color * vec3(texture(material.diffuse_map, TexCoord));
        am_t *= attenuation;
        ambient += am_t;
        
        // Diffuse 
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(current_l.position - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 dif_t = color * (diff * vec3(texture(material.diffuse_map, TexCoord)));
        dif_t *= attenuation;
        diffuse += dif_t;
        
        // Specular
        float specularStrength = 0.5;
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);  
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shine);
        vec3 sp_t = color * (spec * vec3(texture(material.specular_map, TexCoord)));
        sp_t *= attenuation;
        specular += sp_t;
    }

    return vec4(max(ambient_light * base_color.xyz, ambient + diffuse + specular), 1.0);
}

void main() {
	
    vec4 texColor = texture(material.diffuse_map, TexCoord);
    vec4 default_render = LOXOC_default(texColor);
    FragColor = default_render;
}