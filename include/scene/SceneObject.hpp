#ifndef SCENEOBJECT_HPP
#define SCENEOBJECT_HPP

#include "models/Model.hpp"
#include <string>

class SceneObject {
public:
    SceneObject(const std::string& modelPath);
    ~SceneObject();

    bool load();
    void render() const;
    
    void setPosition(float x, float y, float z);
    void setScale(float x, float y, float z);
    void setRotation(float angle, float x, float y, float z);
    
    void getModelMatrix(float* matrix) const;
    void getBoundingBox(float& minX, float& minY, float& minZ,
                       float& maxX, float& maxY, float& maxZ) const;
    void getLocalBoundingBox(float& minX, float& minY, float& minZ,
                            float& maxX, float& maxY, float& maxZ) const;
    
    bool hasTexture() const { return m_model.hasTexture(); }

private:
    std::string m_modelPath;
    Model m_model;
    
    float m_position[3];
    float m_scale[3];
    float m_rotation[4]; // angle, x, y, z
    
    void buildModelMatrix(float* matrix) const;
};

#endif // SCENEOBJECT_HPP

