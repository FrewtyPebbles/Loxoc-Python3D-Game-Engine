#include "CubeMap.h"
#include <stdexcept>

#include <stb_image.h>

#define load_cubemap_img(path, glside) unsigned char* path##_img_data = stbi_load(path##_path.c_str(), &width, &height, &nrChannels, 0);\
    if (path##_img_data) {\
        glTexImage2D(\
            glside, \
            0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, path##_img_data\
        );\
        stbi_image_free(path##_img_data);\
    } else {\
        throw std::runtime_error("Failed to load cubemap texture at: " + path##_path);\
    }


void cubemap::load_textures(string right_path, string left_path, string top_path, string bottom_path, string back_path, string front_path) {
    glGenTextures(1, &this->texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->texture);
    int width, height, nrChannels;
    
    load_cubemap_img(right, GL_TEXTURE_CUBE_MAP_POSITIVE_X);
    load_cubemap_img(left, GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
    load_cubemap_img(top, GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
    load_cubemap_img(bottom, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
    load_cubemap_img(back, GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
    load_cubemap_img(front, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void skybox::render(camera& camera) {
    glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
    mat->data->use_material();
    mat->data->set_uniform("view", camera.view);
    mat->data->set_uniform("projection", camera.projection);
    mat->data->register_uniforms();
    // skybox cube
    glBindVertexArray(vao);
    glActiveTexture(GL_TEXTURE0);
    cube_map->bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);
}