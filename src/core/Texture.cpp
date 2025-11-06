#include "core/Texture.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "core/stb_image.h"
#include <iostream>

Texture::Texture() : m_textureID(0), m_width(0), m_height(0), m_channels(0) {
}

Texture::~Texture() {
    cleanup();
}

void Texture::cleanup() {
    if (m_textureID != 0) {
        glDeleteTextures(1, &m_textureID);
        m_textureID = 0;
    }
}

unsigned char* Texture::loadImageData(const std::string& filepath) {
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(filepath.c_str(), &m_width, &m_height, &m_channels, 0);
    if (!data) {
        std::cerr << "Failed to load texture: " << filepath << std::endl;
        std::cerr << "STB Error: " << stbi_failure_reason() << std::endl;
        return nullptr;
    }
    return data;
}

bool Texture::loadFromFile(const std::string& filepath) {
    cleanup();
    
    unsigned char* data = loadImageData(filepath);
    if (!data) {
        return false;
    }
    
    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    
    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // Determine format
    GLenum format = GL_RGB;
    if (m_channels == 1) {
        format = GL_RED;
    } else if (m_channels == 3) {
        format = GL_RGB;
    } else if (m_channels == 4) {
        format = GL_RGBA;
    }
    
    // Upload texture data
    glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    glBindTexture(GL_TEXTURE_2D, 0);
    
    stbi_image_free(data);
    
    std::cout << "Loaded texture: " << filepath << " (" << m_width << "x" << m_height << ", " << m_channels << " channels)" << std::endl;
    return true;
}

void Texture::bind(unsigned int unit) const {
    if (m_textureID == 0) return;
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
}

void Texture::unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}

