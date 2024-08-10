#pragma once
#include "Vec2.h"
#include "Vec3.h"
#include "Vec4.h"
#include "Quaternion.h"
#include "Material.h"
#include <vector>
#include "util.h"
#include "Camera.h"
#include <string>
#include "glad/gl.h"

using std::vector;

// TODO: add deltatime for decay_rate AND REFACTOR QUAD PRODUCTION INTO GEOMETRY SHADER.

const float quad_array[20] = {
    // Positions        // Texture Coords
    -0.5f,  0.5f, 0.0f,  0.0f, 1.0f,  // Top-left
    0.5f,  0.5f, 0.0f,  1.0f, 1.0f,  // Top-right
    0.5f, -0.5f, 0.0f,  1.0f, 0.0f,  // Bottom-right
    -0.5f, -0.5f, 0.0f,  0.0f, 0.0f   // Bottom-left
};

const unsigned int tex_indices[6] = {
    0, 1, 2,  // First triangle
    2, 3, 0   // Second triangle
};

class particle {
public:
    particle(){}
    particle(vec3 position, vec2 scale, vec3 velocity, vec4 color, float life) : position(position), velocity(velocity), scale(scale), color(color), life(life) {
        create_VAO_init();
    }

    vec3 position, velocity; // position is relative to the emitter
    vec2 scale;
    vec4 color;
    float life;

    inline void create_VAO_init() {
        // Create VAO
        glGenVertexArrays(1, &gl_VAO);
        glBindVertexArray(gl_VAO);

        // Create VBO
        glGenBuffers(1, &gl_VBO);
        glBindBuffer(GL_ARRAY_BUFFER, gl_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quad_array), quad_array, GL_STATIC_DRAW);

        // Create EBO
        glGenBuffers(1, &gl_EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(tex_indices), tex_indices, GL_STATIC_DRAW);

        // Vertex attributes
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }

    inline void create_VAO() {

        // Instance buffer
        glBindVertexArray(gl_VAO);

        // Offset (position)
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(2);
        

        // Color
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(3);

        // Scale
        glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(7 * sizeof(float)));
        glEnableVertexAttribArray(4);

        // life
        glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(9 * sizeof(float)));
        glEnableVertexAttribArray(5);
        
        
        glVertexAttribDivisor(2, 1);
        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);

        glBindVertexArray(0);
    }

    GLuint gl_VAO, gl_VBO, gl_EBO;
};

class emitter {
    /// Uses a set of attributes to determine how the particles are emitted.
public:
    emitter() {}
    emitter(vec3* position, quaternion* direction, vec2* scale_min, vec2* scale_max, size_t rate, float decay_rate, float spread, float velocity_decay, float start_velocity_min, float start_velocity_max, float start_lifetime_min, float start_lifetime_max, vec4* color_min, vec4* color_max, rc_material material) :
        position(position),
        direction(direction),
        scale_min(scale_min),
        scale_max(scale_max),
        rate(rate),
        decay_rate(decay_rate),
        spread(spread),
        velocity_decay(velocity_decay),
        start_velocity_min(start_velocity_min),
        start_velocity_max(start_velocity_max),
        start_lifetime_min(start_lifetime_min),
        start_lifetime_max(start_lifetime_max),
        color_min(color_min),
        color_max(color_max),
        material(material)
    {
        create_particles();
        this->create_VAO();
        for (particle & p : particles)
            p.create_VAO();
    }

    inline void start() {
        emitting = true;
    }

    inline void stop() {
        emitting = false;
    }
    
    inline void render(const camera & cam) {
        if (emitting) {
            material->data->use_material();
            update_instance_batch(cam);
        }
    }

    vector<particle> particles;

    vec3* position;
    quaternion* direction;
    vec2* scale_min;
    vec2* scale_max;
    size_t rate = 30; // particles per max lifetime
    float decay_rate = 0.1f;
    float spread, velocity_decay, start_velocity_min, start_velocity_max, start_lifetime_min, start_lifetime_max;
    vec4* color_min;
    vec4* color_max;
    rc_material material;
    bool emitting = false;
private:

    inline void create_particles() {
        for (size_t i = 0; i < rate; i++)
        {
            particles.push_back(create_particle());
        }
    }

    inline particle create_particle() {
        
        quaternion dir = *direction;
        dir.rotate(vec3(0.0f,1.0f,0.0f), (float)rand_range(-spread, spread));
        dir.rotate(vec3(1.0f,0.0f,0.0f), (float)rand_range(-spread, spread));
        dir.rotate(vec3(0.0f,0.0f,1.0f), (float)rand_range(-spread, spread));
        
        vec4 color(rand_range(color_min->axis.x, color_max->axis.x), rand_range(color_min->axis.y, color_max->axis.y), rand_range(color_min->axis.z, color_max->axis.z), rand_range(color_min->axis.w, color_max->axis.w));
        
        vec2 scale(rand_range(scale_min->axis.x, scale_max->axis.x), rand_range(scale_min->axis.y, scale_max->axis.y));
        
        return particle(
            *position,
            scale,
            dir.get_forward() * rand_range(start_velocity_min, start_velocity_max),
            color,
            rand_range(start_lifetime_min, start_lifetime_max)
        );
    }

    inline void update_particle(particle *p) {
        p->life -= decay_rate;
        if (p->life <= 0.0f) {
            auto new_p = create_particle();
            p->color = new_p.color;
            p->position = new_p.position;
            p->velocity = new_p.velocity;
            p->life = new_p.life;
            p->scale = new_p.scale;
            return;
        }
        p->position += p->velocity;
        p->velocity -= velocity_decay;
    }

    inline void update_instance_batch(const camera & cam) {
        
        vector<float> instance_vbo_update;
        unsigned int i = 0;
        for (particle & p : particles) {
            update_particle(&p);
            instance_vbo_update.push_back(p.position.axis.x);
            instance_vbo_update.push_back(p.position.axis.y);
            instance_vbo_update.push_back(p.position.axis.z);

            instance_vbo_update.push_back(p.color.axis.x);
            instance_vbo_update.push_back(p.color.axis.y);
            instance_vbo_update.push_back(p.color.axis.z);
            instance_vbo_update.push_back(p.color.axis.w);

            instance_vbo_update.push_back(p.scale.axis.x);
            instance_vbo_update.push_back(p.scale.axis.y);

            instance_vbo_update.push_back(p.life);
            
            i++;
        }
        glBindBuffer(GL_ARRAY_BUFFER, gl_instanced_VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, instance_vbo_update.size() * sizeof(float), instance_vbo_update.data());// BUG

        material->data->set_uniform("projection", cam.projection);
        material->data->set_uniform("view", cam.view);
        material->data->set_uniform("sprite", 0);
        material->data->register_uniforms();
        glActiveTexture(GL_TEX_N_ITTER[0]);
        material->data->diffuse_texture->data->bind();

        for (particle &p : particles) {
            glBindVertexArray(p.gl_VAO);
            glDrawElementsInstanced(GL_TRIANGLES, static_cast<unsigned int>(6), GL_UNSIGNED_INT, 0, rate);
            glBindVertexArray(0);
        }
    }

    inline void create_VAO() {
        glGenBuffers(1, &gl_instanced_VBO);
        glBindBuffer(GL_ARRAY_BUFFER, gl_instanced_VBO);
        glBufferData(GL_ARRAY_BUFFER, rate * 10 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
    }

    GLuint gl_VAO, gl_VBO, gl_EBO, gl_instanced_VBO;
};