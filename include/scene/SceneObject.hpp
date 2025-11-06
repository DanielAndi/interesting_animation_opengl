#ifndef SCENEOBJECT_HPP
#define SCENEOBJECT_HPP

#include "models/Model.hpp"
#include <string>

/**
 * @class SceneObject
 * @brief Represents a 3D object in the scene with position, scale, and rotation.
 * 
 * This class wraps a Model and adds transformation properties (position, scale, rotation)
 * that define how the model appears in the scene. It can compute model matrices
 * and bounding boxes in both local and world space.
 */
class SceneObject {
public:
    /**
     * @brief Constructs a scene object with the specified model path.
     * @param modelPath Path to the OBJ model file.
     */
    SceneObject(const std::string& modelPath);
    
    /**
     * @brief Destructor that cleans up the object's resources.
     */
    ~SceneObject();

    /**
     * @brief Loads the model from the file path.
     * @return True if loading succeeded, false otherwise.
     */
    bool load();
    
    /**
     * @brief Renders the object using its current transformation.
     */
    void render() const;
    
    /**
     * @brief Sets the position of the object in world space.
     * @param x The X coordinate.
     * @param y The Y coordinate.
     * @param z The Z coordinate.
     */
    void setPosition(float x, float y, float z);
    
    /**
     * @brief Sets the scale of the object along each axis.
     * @param x Scale factor along the X axis.
     * @param y Scale factor along the Y axis.
     * @param z Scale factor along the Z axis.
     */
    void setScale(float x, float y, float z);
    
    /**
     * @brief Sets the rotation of the object using an axis-angle representation.
     * @param angle Rotation angle in radians.
     * @param x X component of the rotation axis.
     * @param y Y component of the rotation axis.
     * @param z Z component of the rotation axis.
     */
    void setRotation(float angle, float x, float y, float z);
    
    /**
     * @brief Gets the model matrix (transformation matrix) for this object.
     * @param matrix Output parameter for the 16-element matrix array (column-major order).
     */
    void getModelMatrix(float* matrix) const;
    
    /**
     * @brief Gets the axis-aligned bounding box in world space.
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
     * @brief Gets the axis-aligned bounding box in local space (before transformations).
     * @param minX Output parameter for minimum X coordinate.
     * @param minY Output parameter for minimum Y coordinate.
     * @param minZ Output parameter for minimum Z coordinate.
     * @param maxX Output parameter for maximum X coordinate.
     * @param maxY Output parameter for maximum Y coordinate.
     * @param maxZ Output parameter for maximum Z coordinate.
     */
    void getLocalBoundingBox(float& minX, float& minY, float& minZ,
                            float& maxX, float& maxY, float& maxZ) const;
    
    /**
     * @brief Checks if the object's model has an associated texture.
     * @return True if a texture is loaded, false otherwise.
     */
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

