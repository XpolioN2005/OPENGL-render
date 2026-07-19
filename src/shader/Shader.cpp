#include "shader/Shader.h"
#include "shader/ShaderParser.h"

#include <glm/gtc/type_ptr.hpp>
#include <stdexcept>
#include <iostream>

Shader::Shader(const std::string &filepath) {
    Compile(filepath);
}

Shader::~Shader() {
    if (m_Program != 0) {
        glDeleteProgram(m_Program);
    }
}

void Shader::Compile(const std::string &filepath) {
    ShaderParser parser;

    ShaderParser::StageMap stages = parser.Parse(filepath);

    GLuint vertexShader = CompileStage(GL_VERTEX_SHADER, stages[ShaderStage::Vertex]);

    GLuint fragmentShader = CompileStage(GL_FRAGMENT_SHADER, stages[ShaderStage::Fragment]);

    m_Program = glCreateProgram();

    glAttachShader(m_Program, vertexShader);
    glAttachShader(m_Program, fragmentShader);

    glLinkProgram(m_Program);

    GLint success;
    glGetProgramiv(m_Program, GL_LINK_STATUS, &success);

    if (!success) {
        char infoLog[512];

        glGetProgramInfoLog(m_Program, 512, nullptr, infoLog);

        throw std::runtime_error(std::string("Shader linking failed:\n") + infoLog);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

GLuint Shader::CompileStage(GLenum type, const std::string &source) {
    GLuint shader = glCreateShader(type);

    const char *src = source.c_str();

    glShaderSource(shader, 1, &src, nullptr);

    glCompileShader(shader);

    GLint success;

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        char infoLog[512];

        glGetShaderInfoLog(shader, 512, nullptr, infoLog);

        glDeleteShader(shader);

        throw std::runtime_error(std::string("Shader compilation failed:\n") + infoLog);
    }

    return shader;
}

void Shader::Use() const {
    glUseProgram(m_Program);
}

GLuint Shader::GetID() const {
    return m_Program;
}

GLint Shader::GetUniformLocation(const std::string &name) {
    auto it = m_UniformCache.find(name);

    if (it != m_UniformCache.end()) {
        return it->second;
    }

    GLint location = glGetUniformLocation(m_Program, name.c_str());
    m_UniformCache[name] = location;

    return location;
}

void Shader::SetBool(const std::string &name, bool value) {
    GLint location = GetUniformLocation(name);

    if (location != -1) {
        glUniform1i(location, static_cast<int>(value));
    }
}

void Shader::SetInt(const std::string &name, int value) {
    GLint location = GetUniformLocation(name);

    if (location != -1) {
        glUniform1i(location, value);
    }
}

void Shader::SetFloat(const std::string &name, float value) {
    GLint location = GetUniformLocation(name);

    if (location != -1) {
        glUniform1f(location, value);
    }
}

void Shader::SetMat4(const std::string &name, const glm::mat4 &value) {
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}