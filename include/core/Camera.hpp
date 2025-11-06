#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <GL/glew.h>
#include <cmath>

class Camera {
public:
    Camera(float width, float height);
    
    void setPosition(float x, float y, float z);
    void setTarget(float x, float y, float z);
    void setUpVector(float x, float y, float z);
    void setProjection(float fov, float aspect, float nearPlane, float farPlane);
    
    void update();
    
    const float* getViewMatrix() const { return m_viewMatrix; }
    const float* getProjectionMatrix() const { return m_projectionMatrix; }
    
    float getPositionX() const { return m_position[0]; }
    float getPositionY() const { return m_position[1]; }
    float getPositionZ() const { return m_position[2]; }
    
    // Movement methods
    void moveForward(float distance);
    void moveRight(float distance);
    void moveUp(float distance);
    
    // Rotation methods
    void rotateYaw(float angle);
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

