#include "core/Camera.hpp"
#include <cstring>

Camera::Camera(float width, float height) 
    : m_fov(45.0f), m_aspect(width / height), m_nearPlane(0.1f), m_farPlane(100.0f) {
    m_position[0] = 0.0f;
    m_position[1] = 0.0f;
    m_position[2] = 5.0f;
    
    m_target[0] = 0.0f;
    m_target[1] = 0.0f;
    m_target[2] = 0.0f;
    
    m_up[0] = 0.0f;
    m_up[1] = 1.0f;
    m_up[2] = 0.0f;
    
    update();
}

void Camera::setPosition(float x, float y, float z) {
    m_position[0] = x;
    m_position[1] = y;
    m_position[2] = z;
    calculateViewMatrix();
}

void Camera::setTarget(float x, float y, float z) {
    m_target[0] = x;
    m_target[1] = y;
    m_target[2] = z;
    calculateViewMatrix();
}

void Camera::setUpVector(float x, float y, float z) {
    m_up[0] = x;
    m_up[1] = y;
    m_up[2] = z;
    calculateViewMatrix();
}

void Camera::setProjection(float fov, float aspect, float nearPlane, float farPlane) {
    m_fov = fov;
    m_aspect = aspect;
    m_nearPlane = nearPlane;
    m_farPlane = farPlane;
    calculateProjectionMatrix();
}

void Camera::update() {
    calculateViewMatrix();
    calculateProjectionMatrix();
}

void Camera::calculateViewMatrix() {
    // Calculate forward, right, and up vectors
    float forward[3] = {
        m_target[0] - m_position[0],
        m_target[1] - m_position[1],
        m_target[2] - m_position[2]
    };
    
    // Normalize forward
    float length = sqrt(forward[0] * forward[0] + forward[1] * forward[1] + forward[2] * forward[2]);
    if (length > 0.0f) {
        forward[0] /= length;
        forward[1] /= length;
        forward[2] /= length;
    }
    
    // Calculate right vector (forward cross up)
    float right[3] = {
        forward[1] * m_up[2] - forward[2] * m_up[1],
        forward[2] * m_up[0] - forward[0] * m_up[2],
        forward[0] * m_up[1] - forward[1] * m_up[0]
    };
    
    // Normalize right
    length = sqrt(right[0] * right[0] + right[1] * right[1] + right[2] * right[2]);
    if (length > 0.0f) {
        right[0] /= length;
        right[1] /= length;
        right[2] /= length;
    }
    
    // Recalculate up (right cross forward)
    float up[3] = {
        right[1] * forward[2] - right[2] * forward[1],
        right[2] * forward[0] - right[0] * forward[2],
        right[0] * forward[1] - right[1] * forward[0]
    };
    
    // Build view matrix (look-at matrix)
    m_viewMatrix[0] = right[0];
    m_viewMatrix[1] = up[0];
    m_viewMatrix[2] = -forward[0];
    m_viewMatrix[3] = 0.0f;
    
    m_viewMatrix[4] = right[1];
    m_viewMatrix[5] = up[1];
    m_viewMatrix[6] = -forward[1];
    m_viewMatrix[7] = 0.0f;
    
    m_viewMatrix[8] = right[2];
    m_viewMatrix[9] = up[2];
    m_viewMatrix[10] = -forward[2];
    m_viewMatrix[11] = 0.0f;
    
    m_viewMatrix[12] = -(right[0] * m_position[0] + right[1] * m_position[1] + right[2] * m_position[2]);
    m_viewMatrix[13] = -(up[0] * m_position[0] + up[1] * m_position[1] + up[2] * m_position[2]);
    m_viewMatrix[14] = -(-forward[0] * m_position[0] - forward[1] * m_position[1] - forward[2] * m_position[2]);
    m_viewMatrix[15] = 1.0f;
}

void Camera::calculateProjectionMatrix() {
    float tanHalfFov = tan(m_fov * 3.14159265359f / 360.0f);
    float range = m_farPlane - m_nearPlane;
    
    m_projectionMatrix[0] = 1.0f / (m_aspect * tanHalfFov);
    m_projectionMatrix[1] = 0.0f;
    m_projectionMatrix[2] = 0.0f;
    m_projectionMatrix[3] = 0.0f;
    
    m_projectionMatrix[4] = 0.0f;
    m_projectionMatrix[5] = 1.0f / tanHalfFov;
    m_projectionMatrix[6] = 0.0f;
    m_projectionMatrix[7] = 0.0f;
    
    m_projectionMatrix[8] = 0.0f;
    m_projectionMatrix[9] = 0.0f;
    m_projectionMatrix[10] = -(m_farPlane + m_nearPlane) / range;
    m_projectionMatrix[11] = -1.0f;
    
    m_projectionMatrix[12] = 0.0f;
    m_projectionMatrix[13] = 0.0f;
    m_projectionMatrix[14] = -(2.0f * m_farPlane * m_nearPlane) / range;
    m_projectionMatrix[15] = 0.0f;
}

