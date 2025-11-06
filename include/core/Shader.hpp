#ifndef SHADER_HPP
#define SHADER_HPP

#include <GL/glew.h>
#include <string>

class Shader {
public:
    Shader();
    ~Shader();

    bool loadFromFiles(const std::string& vertexPath, const std::string& fragmentPath);
    bool loadFromSource(const std::string& vertexSource, const std::string& fragmentSource);
    void use() const;
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setMat4(const std::string& name, const float* matrix) const;
    void setVec3(const std::string& name, float x, float y, float z) const;

    GLuint getID() const { return m_programID; }

private:
    GLuint m_programID;
    std::string readFile(const std::string& filepath);
    GLuint compileShader(GLenum type, const std::string& source);
    bool linkProgram(GLuint vertexShader, GLuint fragmentShader);
};

#endif // SHADER_HPP

