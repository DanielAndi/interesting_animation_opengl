#ifndef MODEL_HPP
#define MODEL_HPP

#include <GL/glew.h>
#include <string>
#include <vector>
#include "core/Texture.hpp"

struct Vertex {
    float position[3];
    float normal[3];
    float texCoord[2];
};

class Model {
public:
    Model();
    ~Model();

    bool loadFromOBJ(const std::string& filepath);
    void render() const;
    void cleanup();

    // Get bounding box for camera positioning
    void getBoundingBox(float& minX, float& minY, float& minZ, 
                       float& maxX, float& maxY, float& maxZ) const;
    
    bool hasTexture() const { return m_hasTexture; }

private:
    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;
    GLuint m_VAO;
    GLuint m_VBO;
    GLuint m_EBO;
    bool m_initialized;
    Texture m_texture;
    bool m_hasTexture;
    
    void setupBuffers();
    void parseOBJ(const std::string& filepath);
    void parseMTL(const std::string& mtlPath, const std::string& objDir);
    std::string extractDirectory(const std::string& filepath);
};

#endif // MODEL_HPP

