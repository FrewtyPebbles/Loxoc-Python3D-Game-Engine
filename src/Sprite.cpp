#include "Sprite.h"

const GLuint tex_indicies[] = {
    0,1,2,
    2,1,3
};

const GLfloat corners[] = {
    0.0f, 0.0f,   1.0f, 0.0f,
    0.0f, 1.0f,   1.0f, 1.0f
};

void sprite::create_VAO() {
    // get quad array of coords and textures:
    GLfloat quad_array[20];
    size_t i = 0;
    size_t cor = 0;
    for (vec2 vec : quad) {
        quad_array[i] = vec.axis.x;
        quad_array[i+1] = vec.axis.y;
        quad_array[i+2] = 0.0f;
        quad_array[i+3] = corners[cor];
        quad_array[i+4] = corners[cor+1];
        i += 5;
        cor += 2;
    }


    //VAO
    glGenVertexArrays(1, &this->gl_VAO);
    glBindVertexArray(this->gl_VAO);

    //VBO
    glGenBuffers(1, &this->gl_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, this->gl_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad_array), quad_array, GL_DYNAMIC_DRAW);
    
    //EBO
    glGenBuffers(1, &this->gl_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->gl_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(tex_indicies), tex_indicies, GL_STATIC_DRAW);

    // Vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}