#include "Text.h"

// FONT


font::font(const string& font_path, int font_size) {
    
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        throw std::runtime_error("ERROR::FREETYPE: Could not init FreeType Library");
    }

    
    FT_Face face;
    if (FT_New_Face(ft, font_path.c_str(), 0, &face))
    {
        throw std::runtime_error("ERROR::FREETYPE: Failed to load font");
    }

    FT_Set_Pixel_Sizes(face, 0, font_size);

    init_font(face);

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
    setup_buffers();
}

void font::init_font(FT_Face& face) {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction
    
    for (unsigned char c = 0; c < 128; c++)
    {
        // load character glyph 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        // generate texture
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // now store character for later use
        character charac = {
            texture, 
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };
        font_chars[c] = charac;
    }
}

void font::setup_buffers() {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// RENDER TEXT

void text::render(camera& camera) {
    mat->data->use_material(); // Use your shader program

    // Set uniforms like text color
    mat->data->set_uniform("text_color", *color);
    mat->data->set_uniform("projection", matrix4x4(glm::ortho(0.0f, (float)camera.view_width, 0.0f, (float)camera.view_height)));
    mat->data->register_uniforms();
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(font_data->vao);

    


    auto tpos = *position;

    // Iterate through all characters in the text
    for (char c : render_text) {
        // Get the character glyph
        character ch = font_data->font_chars[c]; // Adjust for ASCII offset

        GLfloat xpos = tpos.axis.x + ch.bearing.x * scale->axis.x;
        GLfloat ypos = tpos.axis.y - (ch.size.y - ch.bearing.y) * scale->axis.y;

        GLfloat w = ch.size.x * scale->axis.x;
        GLfloat h = ch.size.y * scale->axis.y;
        // Update VBO for each character
        GLfloat vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };

        // Render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.texture);
        // Update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, font_data->vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // Now advance cursors for next glyph (advance is number of 1/64 pixels)
        tpos.axis.x += (ch.advance >> 6) * scale->axis.x;
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}