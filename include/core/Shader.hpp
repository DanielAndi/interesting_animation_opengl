#ifndef SHADER_HPP
#define SHADER_HPP

#include <GL/glew.h>
#include <string>

/**
 * @class Shader
 * @brief Manages OpenGL shader program compilation, linking, and uniform setting.
 * 
 * This class provides a convenient interface for loading, compiling, and using
 * vertex and fragment shaders. It also provides methods to set uniform values.
 */
class Shader {
public:
    /**
     * @brief Constructs an empty shader program.
     */
    Shader();
    
    /**
     * @brief Destructor that cleans up the shader program.
     */
    ~Shader();

    /**
     * @brief Loads and compiles shaders from files.
     * @param vertexPath Path to the vertex shader source file.
     * @param fragmentPath Path to the fragment shader source file.
     * @return True if loading and compilation succeeded, false otherwise.
     */
    bool loadFromFiles(const std::string& vertexPath, const std::string& fragmentPath);
    
    /**
     * @brief Loads and compiles shaders from source strings.
     * @param vertexSource Vertex shader source code.
     * @param fragmentSource Fragment shader source code.
     * @return True if compilation succeeded, false otherwise.
     */
    bool loadFromSource(const std::string& vertexSource, const std::string& fragmentSource);
    
    /**
     * @brief Activates this shader program for rendering.
     */
    void use() const;
    
    /**
     * @brief Sets a boolean uniform value.
     * @param name Name of the uniform variable in the shader.
     * @param value The boolean value to set.
     */
    void setBool(const std::string& name, bool value) const;
    
    /**
     * @brief Sets an integer uniform value.
     * @param name Name of the uniform variable in the shader.
     * @param value The integer value to set.
     */
    void setInt(const std::string& name, int value) const;
    
    /**
     * @brief Sets a float uniform value.
     * @param name Name of the uniform variable in the shader.
     * @param value The float value to set.
     */
    void setFloat(const std::string& name, float value) const;
    
    /**
     * @brief Sets a 4x4 matrix uniform value.
     * @param name Name of the uniform variable in the shader.
     * @param matrix Pointer to a 16-element array representing the matrix (column-major order).
     */
    void setMat4(const std::string& name, const float* matrix) const;
    
    /**
     * @brief Sets a 3D vector uniform value.
     * @param name Name of the uniform variable in the shader.
     * @param x The X component of the vector.
     * @param y The Y component of the vector.
     * @param z The Z component of the vector.
     */
    void setVec3(const std::string& name, float x, float y, float z) const;

    /**
     * @brief Gets the OpenGL shader program ID.
     * @return The OpenGL program ID.
     */
    GLuint getID() const { return m_programID; }

private:
    GLuint m_programID;
    std::string readFile(const std::string& filepath);
    GLuint compileShader(GLenum type, const std::string& source);
    bool linkProgram(GLuint vertexShader, GLuint fragmentShader);
};

#endif // SHADER_HPP

