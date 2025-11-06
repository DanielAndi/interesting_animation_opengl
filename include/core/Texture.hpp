#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <GL/glew.h>
#include <string>

class Texture {
public:
    Texture();
    ~Texture();

    bool loadFromFile(const std::string& filepath);
    void bind(unsigned int unit = 0) const;
    void unbind() const;
    void cleanup();

    GLuint getID() const { return m_textureID; }
    bool isValid() const { return m_textureID != 0; }

private:
    GLuint m_textureID;
    int m_width;
    int m_height;
    int m_channels;
    
    unsigned char* loadImageData(const std::string& filepath);
};

#endif // TEXTURE_HPP

