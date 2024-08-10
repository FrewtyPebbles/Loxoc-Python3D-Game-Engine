#pragma once
#include <string>
#include <iostream>
#include "glad/gl.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

using std::string;

enum class ShaderType {
    FRAGMENT,
    VERTEX,
    GEOMETRY,
    COMPUTE
};

inline std::ostream& operator<<(std::ostream& os, const ShaderType& self) {
    os << "ShaderType::";
    switch (self)
    {
    case ShaderType::FRAGMENT:
        os << "FRAGMENT";
        break;
    
    case ShaderType::VERTEX:
        os << "VERTEX";
        break;
    case ShaderType::GEOMETRY:
        os << "GEOMETRY";
        break;
    case ShaderType::COMPUTE:
        os << "VERTEX";
        break;
    }
    return os;
}

class shader {
public:
    // do not use empty constructor, only exists for python interop.  Will add throws later.
    shader(){};
    shader(string source, ShaderType type);
    static shader* from_file(string filepath, ShaderType type);
    void compile();
    string source;
    ShaderType type;
    GLuint shader_handle;
};

