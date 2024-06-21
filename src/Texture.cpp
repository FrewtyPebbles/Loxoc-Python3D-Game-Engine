#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
  
texture::texture(string file_path, TextureWraping wrap, TextureFiltering filtering){
    unsigned char * tex = stbi_load(file_path.c_str(), &width, &height, &number_of_channels, 0);
    if (tex) {
        glGenTextures(1, &gl_texture);
        glBindTexture(GL_TEXTURE_2D, gl_texture);
        
        // texture settings:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLint>(wrap));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLint>(wrap));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(filtering));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(filtering));
        
        // texture data:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, number_of_channels > 3 ? GL_RGBA : GL_RGB,
            GL_UNSIGNED_BYTE, tex);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(tex);
    } else {
        
        throw std::runtime_error(std::format("Failed to load texture at \"{}\"\nSTBI log: {}\n\n  HINT: Could be missing \"texture\" folder?", file_path, stbi_failure_reason()));
    }
}

void texture::bind() {
    glBindTexture(GL_TEXTURE_2D, gl_texture);
}