#include "core/Shader.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader() : m_programID(0) {
}

Shader::~Shader() {
    if (m_programID != 0) {
        glDeleteProgram(m_programID);
    }
}

std::string Shader::readFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filepath << std::endl;
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

GLuint Shader::compileShader(GLenum type, const std::string& source) {
    GLuint shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Shader compilation error: " << infoLog << std::endl;
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

bool Shader::linkProgram(GLuint vertexShader, GLuint fragmentShader) {
    m_programID = glCreateProgram();
    glAttachShader(m_programID, vertexShader);
    glAttachShader(m_programID, fragmentShader);
    glLinkProgram(m_programID);

    GLint success;
    glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(m_programID, 512, nullptr, infoLog);
        std::cerr << "Shader linking error: " << infoLog << std::endl;
        glDeleteProgram(m_programID);
        m_programID = 0;
        return false;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return true;
}

bool Shader::loadFromFiles(const std::string& vertexPath, const std::string& fragmentPath) {
    std::string vertexSource = readFile(vertexPath);
    std::string fragmentSource = readFile(fragmentPath);
    
    if (vertexSource.empty() || fragmentSource.empty()) {
        return false;
    }
    
    return loadFromSource(vertexSource, fragmentSource);
}

bool Shader::loadFromSource(const std::string& vertexSource, const std::string& fragmentSource) {
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
    if (vertexShader == 0) return false;

    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);
    if (fragmentShader == 0) {
        glDeleteShader(vertexShader);
        return false;
    }

    return linkProgram(vertexShader, fragmentShader);
}

void Shader::use() const {
    glUseProgram(m_programID);
}

void Shader::setBool(const std::string& name, bool value) const {
    glUniform1i(glGetUniformLocation(m_programID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(m_programID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(m_programID, name.c_str()), value);
}

void Shader::setMat4(const std::string& name, const float* matrix) const {
    glUniformMatrix4fv(glGetUniformLocation(m_programID, name.c_str()), 1, GL_FALSE, matrix);
}

void Shader::setVec3(const std::string& name, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(m_programID, name.c_str()), x, y, z);
}

