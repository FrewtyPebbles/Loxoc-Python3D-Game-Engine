#pragma once
#include <string>
#include "glad/gl.h"
#include "stb_image.h"
#include <stdexcept>
#include <format>

using std::string;

enum class TextureFiltering {
    NEAREST = GL_NEAREST,
    LINEAR = GL_LINEAR
};

enum class TextureWraping {
    REPEAT = GL_REPEAT,
    MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
    CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
    CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER
};

class texture {
public:
    texture(){}
    texture(string file_path, TextureWraping wrap = TextureWraping::REPEAT, TextureFiltering filtering = TextureFiltering::LINEAR){
        unsigned char *data = stbi_load(file_path.c_str(), &width, &height, &number_of_channels, 0);
        if (data) {
            glGenTextures(1, &gl_texture);
            glBindTexture(GL_TEXTURE_2D, gl_texture);
            
            // texture settings:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLint>(wrap));
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLint>(wrap));
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(filtering));
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(filtering));
            
            // texture data:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            stbi_image_free(data);
        } else {
            throw std::runtime_error(std::format("Failed to load texture at \"{}\"\n", file_path));
        }
    }

    void bind();

    int width, height, number_of_channels;
    GLuint gl_texture;
};