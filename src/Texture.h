#pragma once
#include <string>
#include "glad/gl.h"
#include <stdexcept>


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
    texture(string file_path, TextureWraping wrap, TextureFiltering filtering);

    void bind();

    int width = 0, height = 0, number_of_channels = 0;
    GLuint gl_texture;
    string file_path;
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