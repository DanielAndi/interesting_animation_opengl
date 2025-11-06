#include "scene/SceneObject.hpp"
#include <cmath>
#include <cstring>

SceneObject::SceneObject(const std::string& modelPath) 
    : m_modelPath(modelPath) {
    m_position[0] = 0.0f;
    m_position[1] = 0.0f;
    m_position[2] = 0.0f;
    
    m_scale[0] = 1.0f;
    m_scale[1] = 1.0f;
    m_scale[2] = 1.0f;
    
    m_rotation[0] = 0.0f; // angle
    m_rotation[1] = 0.0f; // x
    m_rotation[2] = 1.0f; // y
    m_rotation[3] = 0.0f; // z
}

SceneObject::~SceneObject() {
}

bool SceneObject::load() {
    return m_model.loadFromOBJ(m_modelPath);
}

void SceneObject::render() const {
    m_model.render();
}

void SceneObject::setPosition(float x, float y, float z) {
    m_position[0] = x;
    m_position[1] = y;
    m_position[2] = z;
}

void SceneObject::setScale(float x, float y, float z) {
    m_scale[0] = x;
    m_scale[1] = y;
    m_scale[2] = z;
}

void SceneObject::setRotation(float angle, float x, float y, float z) {
    m_rotation[0] = angle;
    m_rotation[1] = x;
    m_rotation[2] = y;
    m_rotation[3] = z;
}

void SceneObject::buildModelMatrix(float* matrix) const {
    // Initialize as identity matrix
    memset(matrix, 0, 16 * sizeof(float));
    matrix[0] = matrix[5] = matrix[10] = matrix[15] = 1.0f;
    
    // Apply scale
    matrix[0] *= m_scale[0];
    matrix[5] *= m_scale[1];
    matrix[10] *= m_scale[2];
    
    // Apply rotation (simplified - around Y axis for now)
    if (m_rotation[0] != 0.0f && m_rotation[2] == 1.0f) {
        float angle = m_rotation[0] * 3.14159265359f / 180.0f;
        float cosA = cos(angle);
        float sinA = sin(angle);
        
        float temp[16];
        memcpy(temp, matrix, 16 * sizeof(float));
        
        matrix[0] = temp[0] * cosA - temp[8] * sinA;
        matrix[2] = temp[0] * sinA + temp[8] * cosA;
        matrix[8] = temp[2] * cosA - temp[10] * sinA;
        matrix[10] = temp[2] * sinA + temp[10] * cosA;
    }
    
    // Apply translation
    matrix[12] = m_position[0];
    matrix[13] = m_position[1];
    matrix[14] = m_position[2];
}

void SceneObject::getModelMatrix(float* matrix) const {
    buildModelMatrix(matrix);
}

void SceneObject::getBoundingBox(float& minX, float& minY, float& minZ,
                                float& maxX, float& maxY, float& maxZ) const {
    m_model.getBoundingBox(minX, minY, minZ, maxX, maxY, maxZ);
    
    // Transform bounding box by scale and position
    float centerX = (minX + maxX) * 0.5f;
    float centerY = (minY + maxY) * 0.5f;
    float centerZ = (minZ + maxZ) * 0.5f;
    
    float halfSizeX = (maxX - minX) * 0.5f * m_scale[0];
    float halfSizeY = (maxY - minY) * 0.5f * m_scale[1];
    float halfSizeZ = (maxZ - minZ) * 0.5f * m_scale[2];
    
    minX = centerX * m_scale[0] + m_position[0] - halfSizeX;
    maxX = centerX * m_scale[0] + m_position[0] + halfSizeX;
    minY = centerY * m_scale[1] + m_position[1] - halfSizeY;
    maxY = centerY * m_scale[1] + m_position[1] + halfSizeY;
    minZ = centerZ * m_scale[2] + m_position[2] - halfSizeZ;
    maxZ = centerZ * m_scale[2] + m_position[2] + halfSizeZ;
}

void SceneObject::getLocalBoundingBox(float& minX, float& minY, float& minZ,
                                      float& maxX, float& maxY, float& maxZ) const {
    // Get the raw model bounding box without transformations
    m_model.getBoundingBox(minX, minY, minZ, maxX, maxY, maxZ);
}

