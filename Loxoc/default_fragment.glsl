#version 330 core


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

struct SpotLight {
    vec3 position;
    mat4 rotation;
    vec3 color;
    float cutOff;
    float outerCutOff;
    float constant;
    float linear;
    float quadratic;
    float intensity;
};


#define MAX_LIGHTS 15

uniform PointLight point_lights[MAX_LIGHTS];
uniform int total_point_lights;

uniform DirectionalLight directional_lights[MAX_LIGHTS];
uniform int total_directional_lights;

uniform SpotLight spot_lights[MAX_LIGHTS];
uniform int total_spot_lights;

uniform vec3 viewPos;
uniform vec3 ambient_light;

uniform Material material;


in vec3 FragPos;  
in vec3 Normal;
in vec2 TexCoord;

out vec4 FragColor;

vec3 extractDirection(mat4 matrix) {
    return normalize(vec3(matrix[2][0], matrix[2][1], matrix[2][2]));
}

vec4 LOXOC_default(vec4 base_color) {
	vec3 ambient = vec3(0.0);
    vec3 diffuse = vec3(0.0);
    vec3 specular = vec3(0.0);
    vec3 norm = normalize(Normal);
    for(int i = 0; i < total_directional_lights; i++) { // DIRECTIONAL LIGHTS
        DirectionalLight current_l = directional_lights[i];

        vec3 lightDir = extractDirection(current_l.rotation);
        
        // Ambient
        vec3 am_t = current_l.color * vec3(texture(material.diffuse_map, TexCoord));
        ambient += am_t;
        
        // Diffuse 
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 dif_t = current_l.color * (diff * vec3(texture(material.diffuse_map, TexCoord)));
        diffuse += dif_t;
        
        // Specular
        float specularStrength = 0.5;
        vec3 viewDir    = normalize(viewPos - FragPos);
        vec3 halfwayDir = normalize(lightDir + viewDir);
        float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shine);
        vec3 sp_t = current_l.color * (spec * vec3(texture(material.specular_map, TexCoord)));
        specular += sp_t;
    }

    for(int i = 0; i < total_point_lights; i++) { // POINT LIGHTS
		PointLight current_l = point_lights[i];
		float l_distance = length(current_l.position - FragPos);
        float attenuation = 1.0 / (current_l.constant + current_l.linear * l_distance + current_l.quadratic * (l_distance * l_distance));
        vec3 color = current_l.color * current_l.intensity;

        vec3 lightDir = normalize(current_l.position - FragPos);

        // Ambient
        vec3 am_t = color * vec3(texture(material.diffuse_map, TexCoord));
        am_t *= attenuation;
        ambient += am_t;
        
        // Diffuse
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 dif_t = color * (diff * vec3(texture(material.diffuse_map, TexCoord)));
        dif_t *= attenuation;
        diffuse += dif_t;
        
        // Specular
        float specularStrength = 0.5;
        vec3 viewDir    = normalize(viewPos - FragPos);
        vec3 halfwayDir = normalize(lightDir + viewDir);
        float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shine);
        vec3 sp_t = color * (spec * vec3(texture(material.specular_map, TexCoord)));
        sp_t *= attenuation;
        specular += sp_t;
    }

    for(int i = 0; i < total_spot_lights; i++) { // SPOT LIGHTS
        SpotLight current_l = spot_lights[i];
        

        vec3 lightDir = normalize(current_l.position - FragPos);

        float theta = dot(lightDir, extractDirection(current_l.rotation));
        float epsilon = current_l.cutOff - current_l.outerCutOff;
        float intensity = clamp((theta - current_l.outerCutOff) / epsilon, 0.0, 1.0);

        float l_distance = length(current_l.position - FragPos);
        float attenuation = 1.0 / (current_l.constant + current_l.linear * l_distance + current_l.quadratic * (l_distance * l_distance));
        vec3 color = current_l.color * current_l.intensity;
    
        if (theta > current_l.cutOff)
        {       
            vec3 diffuse_tex_val = vec3(texture(material.diffuse_map, TexCoord));
            // Ambient
            vec3 am_t = color * diffuse_tex_val;
            am_t *= attenuation;
            ambient += am_t;
            
            // Diffuse 
            float diff = max(dot(norm, lightDir), 0.0);
            vec3 dif_t = color * (diff * diffuse_tex_val);
            dif_t *= attenuation;
            diffuse += dif_t;
            
            // Specular
            float specularStrength = 0.5;
            vec3 viewDir    = normalize(viewPos - FragPos);
            vec3 halfwayDir = normalize(lightDir + viewDir);
            float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shine);
            vec3 sp_t = color * (spec * vec3(texture(material.specular_map, TexCoord)));
            sp_t *= attenuation;
            specular += sp_t;
        }
    }

    return vec4(max(ambient_light * base_color.xyz, ambient + diffuse + specular), 1.0);
}

void main() {
	
    vec4 texColor = texture(material.diffuse_map, TexCoord);
    vec4 default_render = LOXOC_default(texColor);
    FragColor = default_render;
}
