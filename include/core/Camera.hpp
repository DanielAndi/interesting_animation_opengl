#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <GL/glew.h>
#include <cmath>

/**
 * @class Camera
 * @brief Manages camera position, orientation, and projection matrices for 3D rendering.
 * 
 * This class provides functionality for camera movement, rotation, and projection setup.
 * It maintains view and projection matrices that can be used by shaders for rendering.
 */
class Camera {
public:
    /**
     * @brief Constructs a camera with the specified viewport dimensions.
     * @param width The width of the viewport in pixels.
     * @param height The height of the viewport in pixels.
     */
    Camera(float width, float height);
    
    /**
     * @brief Sets the camera's position in world space.
     * @param x The X coordinate of the camera position.
     * @param y The Y coordinate of the camera position.
     * @param z The Z coordinate of the camera position.
     */
    void setPosition(float x, float y, float z);
    
    /**
     * @brief Sets the target point the camera is looking at.
     * @param x The X coordinate of the target point.
     * @param y The Y coordinate of the target point.
     * @param z The Z coordinate of the target point.
     */
    void setTarget(float x, float y, float z);
    
    /**
     * @brief Sets the up vector for the camera orientation.
     * @param x The X component of the up vector.
     * @param y The Y component of the up vector.
     * @param z The Z component of the up vector.
     */
    void setUpVector(float x, float y, float z);
    
    /**
     * @brief Sets the projection matrix parameters.
     * @param fov Field of view angle in degrees.
     * @param aspect Aspect ratio (width/height) of the viewport.
     * @param nearPlane Distance to the near clipping plane.
     * @param farPlane Distance to the far clipping plane.
     */
    void setProjection(float fov, float aspect, float nearPlane, float farPlane);
    
    /**
     * @brief Updates the camera matrices. Call this after modifying camera properties.
     */
    void update();
    
    /**
     * @brief Gets a pointer to the view matrix (16-element array, column-major order).
     * @return Pointer to the view matrix array.
     */
    const float* getViewMatrix() const { return m_viewMatrix; }
    
    /**
     * @brief Gets a pointer to the projection matrix (16-element array, column-major order).
     * @return Pointer to the projection matrix array.
     */
    const float* getProjectionMatrix() const { return m_projectionMatrix; }
    
    /**
     * @brief Gets the X coordinate of the camera position.
     * @return The X coordinate.
     */
    float getPositionX() const { return m_position[0]; }
    
    /**
     * @brief Gets the Y coordinate of the camera position.
     * @return The Y coordinate.
     */
    float getPositionY() const { return m_position[1]; }
    
    /**
     * @brief Gets the Z coordinate of the camera position.
     * @return The Z coordinate.
     */
    float getPositionZ() const { return m_position[2]; }
    
    /**
     * @brief Moves the camera forward along its current viewing direction.
     * @param distance The distance to move forward (can be negative to move backward).
     */
    void moveForward(float distance);
    
    /**
     * @brief Moves the camera right relative to its current orientation.
     * @param distance The distance to move right (can be negative to move left).
     */
    void moveRight(float distance);
    
    /**
     * @brief Moves the camera up along the up vector.
     * @param distance The distance to move up (can be negative to move down).
     */
    void moveUp(float distance);
    
    /**
     * @brief Rotates the camera horizontally (yaw rotation).
     * @param angle The rotation angle in radians.
     */
    void rotateYaw(float angle);
    
    /**
     * @brief Rotates the camera vertically (pitch rotation).
     * @param angle The rotation angle in radians.
     */
    void rotatePitch(float angle);

private:
    void calculateViewMatrix();
    void calculateProjectionMatrix();
    
    float m_position[3];
    float m_target[3];
    float m_up[3];
    float m_viewMatrix[16];
    float m_projectionMatrix[16];
    float m_fov;
    float m_aspect;
    float m_nearPlane;
    float m_farPlane;
};

#endif // CAMERA_HPP

