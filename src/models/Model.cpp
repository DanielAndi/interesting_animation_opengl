#include "models/Model.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cstring>

Model::Model() : m_VAO(0), m_VBO(0), m_EBO(0), m_initialized(false), m_hasTexture(false) {
}

Model::~Model() {
    cleanup();
}

void Model::cleanup() {
    if (m_VAO != 0) {
        glDeleteVertexArrays(1, &m_VAO);
        m_VAO = 0;
    }
    if (m_VBO != 0) {
        glDeleteBuffers(1, &m_VBO);
        m_VBO = 0;
    }
    if (m_EBO != 0) {
        glDeleteBuffers(1, &m_EBO);
        m_EBO = 0;
    }
    m_initialized = false;
}

bool Model::loadFromOBJ(const std::string& filepath) {
    m_vertices.clear();
    m_indices.clear();
    m_hasTexture = false;
    
    std::string objDir = extractDirectory(filepath);
    parseOBJ(filepath);
    
    if (m_vertices.empty()) {
        std::cerr << "Failed to load model: " << filepath << std::endl;
        return false;
    }
    
    setupBuffers();
    return true;
}

void Model::parseOBJ(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Failed to open OBJ file: " << filepath << std::endl;
        return;
    }
    
    std::vector<float> positions;
    std::vector<float> normals;
    std::vector<float> texCoords;
    std::vector<std::string> faces;
    std::string mtlPath;
    std::string objDir = extractDirectory(filepath);
    
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        
        std::istringstream iss(line);
        std::string type;
        iss >> type;
        
        if (type == "mtllib") {
            std::string mtlFile;
            iss >> mtlFile;
            mtlPath = objDir + "/" + mtlFile;
        } else if (type == "v") {
            float x, y, z;
            if (iss >> x >> y >> z) {
                positions.push_back(x);
                positions.push_back(y);
                positions.push_back(z);
            }
        } else if (type == "vn") {
            float x, y, z;
            if (iss >> x >> y >> z) {
                normals.push_back(x);
                normals.push_back(y);
                normals.push_back(z);
            }
        } else if (type == "vt") {
            float u, v;
            if (iss >> u >> v) {
                texCoords.push_back(u);
                texCoords.push_back(v);
            }
        } else if (type == "f") {
            faces.push_back(line);
        }
    }
    
    // Parse faces and create vertices
    for (const auto& face : faces) {
        std::istringstream iss(face);
        std::string type;
        iss >> type; // skip "f"
        
        std::vector<Vertex> faceVertices;
        std::string vertex;
        
        // Collect all vertices in the face
        while (iss >> vertex) {
            unsigned int posIdx = 0, texIdx = 0, normIdx = 0;
            
            // Parse vertex format: pos/tex/norm or pos//norm or pos
            size_t firstSlash = vertex.find('/');
            if (firstSlash != std::string::npos) {
                posIdx = std::stoul(vertex.substr(0, firstSlash)) - 1;
                
                size_t secondSlash = vertex.find('/', firstSlash + 1);
                if (secondSlash != std::string::npos) {
                    if (secondSlash > firstSlash + 1) {
                        texIdx = std::stoul(vertex.substr(firstSlash + 1, secondSlash - firstSlash - 1)) - 1;
                    }
                    normIdx = std::stoul(vertex.substr(secondSlash + 1)) - 1;
                } else {
                    texIdx = std::stoul(vertex.substr(firstSlash + 1)) - 1;
                }
            } else {
                posIdx = std::stoul(vertex) - 1;
            }
            
            // Create vertex
            Vertex v;
            if (posIdx * 3 + 2 < positions.size()) {
                v.position[0] = positions[posIdx * 3];
                v.position[1] = positions[posIdx * 3 + 1];
                v.position[2] = positions[posIdx * 3 + 2];
            }
            
            if (normIdx * 3 + 2 < normals.size()) {
                v.normal[0] = normals[normIdx * 3];
                v.normal[1] = normals[normIdx * 3 + 1];
                v.normal[2] = normals[normIdx * 3 + 2];
            } else {
                v.normal[0] = 0.0f;
                v.normal[1] = 1.0f;
                v.normal[2] = 0.0f;
            }
            
            if (texIdx * 2 + 1 < texCoords.size()) {
                v.texCoord[0] = texCoords[texIdx * 2];
                v.texCoord[1] = texCoords[texIdx * 2 + 1];
            } else {
                v.texCoord[0] = 0.0f;
                v.texCoord[1] = 0.0f;
            }
            
            faceVertices.push_back(v);
        }
        
        // Handle triangles and quads (split quads into triangles)
        if (faceVertices.size() >= 3) {
            // First triangle: 0, 1, 2
            for (int i = 0; i < 3; i++) {
                const Vertex& v = faceVertices[i];
                auto it = std::find_if(m_vertices.begin(), m_vertices.end(),
                    [&v](const Vertex& existing) {
                        return std::abs(existing.position[0] - v.position[0]) < 0.0001f &&
                               std::abs(existing.position[1] - v.position[1]) < 0.0001f &&
                               std::abs(existing.position[2] - v.position[2]) < 0.0001f &&
                               std::abs(existing.normal[0] - v.normal[0]) < 0.0001f &&
                               std::abs(existing.normal[1] - v.normal[1]) < 0.0001f &&
                               std::abs(existing.normal[2] - v.normal[2]) < 0.0001f;
                    });
                
                if (it != m_vertices.end()) {
                    m_indices.push_back(std::distance(m_vertices.begin(), it));
                } else {
                    m_indices.push_back(m_vertices.size());
                    m_vertices.push_back(v);
                }
            }
            
            // If quad, add second triangle: 0, 2, 3
            if (faceVertices.size() >= 4) {
                int quadIndices[3] = {0, 2, 3};
                for (int i = 0; i < 3; i++) {
                    const Vertex& v = faceVertices[quadIndices[i]];
                    auto it = std::find_if(m_vertices.begin(), m_vertices.end(),
                        [&v](const Vertex& existing) {
                            return std::abs(existing.position[0] - v.position[0]) < 0.0001f &&
                                   std::abs(existing.position[1] - v.position[1]) < 0.0001f &&
                                   std::abs(existing.position[2] - v.position[2]) < 0.0001f &&
                                   std::abs(existing.normal[0] - v.normal[0]) < 0.0001f &&
                                   std::abs(existing.normal[1] - v.normal[1]) < 0.0001f &&
                                   std::abs(existing.normal[2] - v.normal[2]) < 0.0001f;
                        });
                    
                    if (it != m_vertices.end()) {
                        m_indices.push_back(std::distance(m_vertices.begin(), it));
                    } else {
                        m_indices.push_back(m_vertices.size());
                        m_vertices.push_back(v);
                    }
                }
            }
        }
    }
    
    std::cout << "Loaded model: " << m_vertices.size() << " vertices, " 
              << m_indices.size() << " indices" << std::endl;
    
    // Parse MTL file if found
    if (!mtlPath.empty()) {
        parseMTL(mtlPath, objDir);
    }
}

std::string Model::extractDirectory(const std::string& filepath) {
    size_t lastSlash = filepath.find_last_of("/\\");
    if (lastSlash != std::string::npos) {
        return filepath.substr(0, lastSlash);
    }
    return ".";
}

void Model::parseMTL(const std::string& mtlPath, const std::string& objDir) {
    std::ifstream file(mtlPath);
    if (!file.is_open()) {
        std::cerr << "Failed to open MTL file: " << mtlPath << std::endl;
        return;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        
        std::istringstream iss(line);
        std::string type;
        iss >> type;
        
        if (type == "map_Kd") {
            std::string texturePath;
            iss >> texturePath;
            
            // Extract just the filename if it's a full path
            size_t lastSlash = texturePath.find_last_of("/\\");
            std::string textureFile = (lastSlash != std::string::npos) ? 
                texturePath.substr(lastSlash + 1) : texturePath;
            
            std::string fullTexturePath = objDir + "/" + textureFile;
            
            if (m_texture.loadFromFile(fullTexturePath)) {
                m_hasTexture = true;
                std::cout << "Loaded texture: " << fullTexturePath << std::endl;
            } else {
                // Try with the original path
                if (m_texture.loadFromFile(texturePath)) {
                    m_hasTexture = true;
                }
            }
            break; // Load first texture found
        }
    }
}

void Model::setupBuffers() {
    cleanup();
    
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);
    
    glBindVertexArray(m_VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), 
                 m_vertices.data(), GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int),
                 m_indices.data(), GL_STATIC_DRAW);
    
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);
    
    // Texture coordinate attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
    glEnableVertexAttribArray(2);
    
    glBindVertexArray(0);
    m_initialized = true;
}

void Model::render() const {
    if (!m_initialized) return;
    
    // Bind texture if available
    if (m_hasTexture && m_texture.isValid()) {
        m_texture.bind(0);
    }
    
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    
    if (m_hasTexture && m_texture.isValid()) {
        m_texture.unbind();
    }
}

void Model::getBoundingBox(float& minX, float& minY, float& minZ,
                          float& maxX, float& maxY, float& maxZ) const {
    if (m_vertices.empty()) {
        minX = minY = minZ = maxX = maxY = maxZ = 0.0f;
        return;
    }
    
    minX = maxX = m_vertices[0].position[0];
    minY = maxY = m_vertices[0].position[1];
    minZ = maxZ = m_vertices[0].position[2];
    
    for (const auto& vertex : m_vertices) {
        minX = std::min(minX, vertex.position[0]);
        maxX = std::max(maxX, vertex.position[0]);
        minY = std::min(minY, vertex.position[1]);
        maxY = std::max(maxY, vertex.position[1]);
        minZ = std::min(minZ, vertex.position[2]);
        maxZ = std::max(maxZ, vertex.position[2]);
    }
}

