#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <GL/glew.h>
#include <string>

/**
 * @class Texture
 * @brief Manages OpenGL texture loading, binding, and cleanup.
 * 
 * This class provides functionality for loading image files as OpenGL textures
 * and managing their lifecycle. It uses stb_image for image loading.
 */
class Texture {
public:
    /**
     * @brief Constructs an empty texture object.
     */
    Texture();
    
    /**
     * @brief Destructor that cleans up the texture if it was loaded.
     */
    ~Texture();

    /**
     * @brief Loads a texture from an image file.
     * @param filepath Path to the image file to load.
     * @return True if loading succeeded, false otherwise.
     */
    bool loadFromFile(const std::string& filepath);
    
    /**
     * @brief Binds the texture to the specified texture unit.
     * @param unit The texture unit to bind to (default is 0).
     */
    void bind(unsigned int unit = 0) const;
    
    /**
     * @brief Unbinds the texture from the current texture unit.
     */
    void unbind() const;
    
    /**
     * @brief Cleans up the texture and frees OpenGL resources.
     */
    void cleanup();

    /**
     * @brief Gets the OpenGL texture ID.
     * @return The OpenGL texture ID, or 0 if not loaded.
     */
    GLuint getID() const { return m_textureID; }
    
    /**
     * @brief Checks if the texture is valid (has been loaded).
     * @return True if the texture is loaded and valid, false otherwise.
     */
    bool isValid() const { return m_textureID != 0; }

private:
    GLuint m_textureID;
    int m_width;
    int m_height;
    int m_channels;
    
    unsigned char* loadImageData(const std::string& filepath);
};

#endif // TEXTURE_HPP

