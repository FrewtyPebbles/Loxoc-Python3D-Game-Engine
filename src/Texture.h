#pragma once
#include <string>
#include "glad/gl.h"
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
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
    texture(string file_path, TextureWraping wrap, TextureFiltering filtering){
        cv::Mat tex = cv::imread(file_path);
        if (!tex.empty()) {
            width = tex.cols;
            height = tex.rows;
            number_of_channels = tex.channels();
            glGenTextures(1, &gl_texture);
            glBindTexture(GL_TEXTURE_2D, gl_texture);
            
            // texture settings:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLint>(wrap));
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLint>(wrap));
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(filtering));
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(filtering));
            
            // texture data:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, number_of_channels > 3 ? GL_BGRA : GL_BGR,
                GL_UNSIGNED_BYTE, tex.data);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            throw std::runtime_error(std::format("Failed to load texture at \"{}\"\n\n  HINT: Could be missing \"texture\" folder?", file_path));
        }
    }

    void bind();

    int width, height, number_of_channels;
    GLuint gl_texture;
};

const int GL_TEX_N_ITTER[] = {
    GL_TEXTURE0,
    GL_TEXTURE1,
    GL_TEXTURE2,
    GL_TEXTURE3,
    GL_TEXTURE4,
    GL_TEXTURE5,
    GL_TEXTURE6,
    GL_TEXTURE7,
    GL_TEXTURE8,
    GL_TEXTURE9,
    GL_TEXTURE10,
    GL_TEXTURE11,
    GL_TEXTURE12,
    GL_TEXTURE13,
    GL_TEXTURE14,
    GL_TEXTURE15,
    GL_TEXTURE16,
    GL_TEXTURE17,
    GL_TEXTURE18,
    GL_TEXTURE19,
    GL_TEXTURE20,
    GL_TEXTURE21,
    GL_TEXTURE22,
    GL_TEXTURE23,
    GL_TEXTURE24,
    GL_TEXTURE25,
    GL_TEXTURE26,
    GL_TEXTURE27,
    GL_TEXTURE28,
    GL_TEXTURE29,
    GL_TEXTURE30
};