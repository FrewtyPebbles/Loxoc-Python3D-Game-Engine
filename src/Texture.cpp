#include "Texture.h"

void texture::bind() {
    glBindTexture(GL_TEXTURE_2D, gl_texture);
}