#ifndef MODEL_HPP
#define MODEL_HPP

#include <GL/glew.h>
#include <string>
#include <vector>
#include "core/Texture.hpp"

/**
 * @struct Vertex
 * @brief Represents a single vertex with position, normal, and texture coordinates.
 */
struct Vertex {
    float position[3];  ///< 3D position coordinates (x, y, z)
    float normal[3];    ///< Normal vector (x, y, z)
    float texCoord[2];  ///< Texture coordinates (u, v)
};

/**
 * @class Model
 * @brief Loads and renders 3D models from OBJ files.
 * 
 * This class handles loading 3D models from OBJ format files, including
 * parsing geometry, normals, texture coordinates, and associated material files.
 * It manages OpenGL buffers for efficient rendering.
 */
class Model {
public:
    /**
     * @brief Constructs an empty model.
     */
    Model();
    
    /**
     * @brief Destructor that cleans up OpenGL resources.
     */
    ~Model();

    /**
     * @brief Loads a 3D model from an OBJ file.
     * @param filepath Path to the OBJ file to load.
     * @return True if loading succeeded, false otherwise.
     */
    bool loadFromOBJ(const std::string& filepath);
    
    /**
     * @brief Renders the model using the current OpenGL state.
     */
    void render() const;
    
    /**
     * @brief Cleans up OpenGL buffers and resources.
     */
    void cleanup();

    /**
     * @brief Gets the axis-aligned bounding box of the model.
     * @param minX Output parameter for minimum X coordinate.
     * @param minY Output parameter for minimum Y coordinate.
     * @param minZ Output parameter for minimum Z coordinate.
     * @param maxX Output parameter for maximum X coordinate.
     * @param maxY Output parameter for maximum Y coordinate.
     * @param maxZ Output parameter for maximum Z coordinate.
     */
    void getBoundingBox(float& minX, float& minY, float& minZ, 
                       float& maxX, float& maxY, float& maxZ) const;
    
    /**
     * @brief Checks if the model has an associated texture.
     * @return True if a texture is loaded, false otherwise.
     */
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

