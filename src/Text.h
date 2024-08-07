#pragma once
#include <string>
#include <vector>
#include <fstream>

#include "Vec2.h"
#include "Vec3.h"
#include "Material.h"
#include "Camera.h"
#include "Matrix.h"

#include "glad/gl.h"

#include <ft2build.h>
#include FT_FREETYPE_H

using std::string;
using std::vector;

struct character {
    GLuint texture;  // ID handle of the glyph texture
    glm::ivec2 size;   // Size of glyph
    glm::ivec2 bearing; // Offset from baseline to left/top of glyph
    GLuint advance;    // Offset to advance to next glyph
};

class font {
public:
    font() {}
    font(const string& font_path, int font_size);

    void init_font(FT_Face& face);

    void setup_buffers();

friend class text;
private:
    
    character font_chars[128];
    unsigned int vao, vbo;
    int atlas_width, atlas_height;
};

class text {
public:
    text() {}
    text(string render_text, font* font_data, vec4* color, vec2* position, vec2* scale, float rotation, rc_material mat = nullptr) :
        render_text(render_text),
        font_data(font_data),
        position(position),
        rotation(rotation),
        scale(scale),
        mat(mat),
        color(color)
    {}

    void render(camera& camera);

    inline matrix4x4 get_model_matrix() {
        matrix4x4 model = matrix4x4(1.0f).translate(vec3(*position, 0.0f));
        model = model.rotate(rotation, vec3(0.0f, 0.0f, 1.0f));
        model = model.scale(vec3(*scale, 1.0f));
        model_matrix = model;
        return model;
    }

    string render_text;
    font* font_data;
    vec2* position;
    float rotation;
    vec2* scale;
    vec4* color;
    rc_material mat;
    matrix4x4 model_matrix;
};