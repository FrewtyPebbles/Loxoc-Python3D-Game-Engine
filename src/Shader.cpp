#include "Shader.h"
#include <fstream>
#include <sstream>

shader::shader(string source, ShaderType type) : source(source), type(type) {
    this->compile();
}

shader* shader::from_file(string filepath, ShaderType type) {
    std::ifstream fileStream(filepath);
    if (!fileStream.is_open()) {
        throw std::runtime_error("Could not open file: " + filepath);
    }

    std::stringstream buffer;
    buffer << fileStream.rdbuf() << std::endl;
    fileStream.close();
    return new shader(buffer.str(), type);
}

void shader::compile() {
    switch (this->type)
    {
    case ShaderType::FRAGMENT:
        this->shader_handle = glCreateShader(GL_FRAGMENT_SHADER);
        break;

    case ShaderType::VERTEX:
        this->shader_handle = glCreateShader(GL_VERTEX_SHADER);
        break;
    default:
        break;
    }
    const char * _src = this->source.c_str();
    glShaderSource(this->shader_handle, 1, &_src, NULL);
    glCompileShader(this->shader_handle);

    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(this->shader_handle, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(this->shader_handle, 512, NULL, infoLog);
        std::stringstream ss;
        ss << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << "\n";
        std::cerr << ss.str();
        throw std::runtime_error(ss.str());
    }
}

