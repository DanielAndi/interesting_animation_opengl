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

void Camera::moveForward(float distance) {
    // Calculate forward direction
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
    
    // Move position and target
    m_position[0] += forward[0] * distance;
    m_position[1] += forward[1] * distance;
    m_position[2] += forward[2] * distance;
    m_target[0] += forward[0] * distance;
    m_target[1] += forward[1] * distance;
    m_target[2] += forward[2] * distance;
    
    calculateViewMatrix();
}

void Camera::moveRight(float distance) {
    // Calculate forward and right directions
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
    
    // Move position and target
    m_position[0] += right[0] * distance;
    m_position[1] += right[1] * distance;
    m_position[2] += right[2] * distance;
    m_target[0] += right[0] * distance;
    m_target[1] += right[1] * distance;
    m_target[2] += right[2] * distance;
    
    calculateViewMatrix();
}

void Camera::moveUp(float distance) {
    // Move position and target along the up vector
    m_position[0] += m_up[0] * distance;
    m_position[1] += m_up[1] * distance;
    m_position[2] += m_up[2] * distance;
    m_target[0] += m_up[0] * distance;
    m_target[1] += m_up[1] * distance;
    m_target[2] += m_up[2] * distance;
    
    calculateViewMatrix();
}

void Camera::rotateYaw(float angle) {
    // Calculate forward direction (from position to target)
    float forward[3] = {
        m_target[0] - m_position[0],
        m_target[1] - m_position[1],
        m_target[2] - m_position[2]
    };
    
    float length = sqrt(forward[0] * forward[0] + forward[1] * forward[1] + forward[2] * forward[2]);
    if (length <= 0.0f) return;
    
    // Normalize
    forward[0] /= length;
    forward[1] /= length;
    forward[2] /= length;
    
    // Project forward onto horizontal plane (remove vertical component)
    float horizontal[3] = {forward[0], 0.0f, forward[2]};
    float horizontalLength = sqrt(horizontal[0] * horizontal[0] + horizontal[2] * horizontal[2]);
    
    if (horizontalLength <= 0.0f) {
        // Looking straight up/down, can't rotate yaw
        return;
    }
    
    // Normalize horizontal
    horizontal[0] /= horizontalLength;
    horizontal[2] /= horizontalLength;
    
    // Rotate horizontal vector around Y-axis (yaw rotation)
    float angleRad = angle * 3.14159265359f / 180.0f;
    float cosAngle = cos(angleRad);
    float sinAngle = sin(angleRad);
    
    // Rotate horizontal component around Y-axis
    float newHorizontal[3] = {
        horizontal[0] * cosAngle - horizontal[2] * sinAngle,
        0.0f,
        horizontal[0] * sinAngle + horizontal[2] * cosAngle
    };
    
    // Combine with preserved vertical component
    float verticalComponent = forward[1];
    float newHorizontalLength = sqrt(newHorizontal[0] * newHorizontal[0] + newHorizontal[2] * newHorizontal[2]);
    
    // Reconstruct forward vector maintaining the same length
    float newForwardLength = sqrt(newHorizontalLength * newHorizontalLength + verticalComponent * verticalComponent);
    float scale = length / newForwardLength;
    
    float newForward[3] = {
        newHorizontal[0] * scale,
        verticalComponent * scale,
        newHorizontal[2] * scale
    };
    
    // Update target
    m_target[0] = m_position[0] + newForward[0];
    m_target[1] = m_position[1] + newForward[1];
    m_target[2] = m_position[2] + newForward[2];
    
    calculateViewMatrix();
}

void Camera::rotatePitch(float angle) {
    // Calculate forward direction (from position to target)
    float forward[3] = {
        m_target[0] - m_position[0],
        m_target[1] - m_position[1],
        m_target[2] - m_position[2]
    };
    
    float length = sqrt(forward[0] * forward[0] + forward[1] * forward[1] + forward[2] * forward[2]);
    if (length <= 0.0f) return;
    
    // Normalize
    forward[0] /= length;
    forward[1] /= length;
    forward[2] /= length;
    
    // Get current pitch angle
    float currentPitch = asin(forward[1]);
    
    // Apply rotation and clamp to avoid gimbal lock
    float angleRad = angle * 3.14159265359f / 180.0f;
    float newPitch = currentPitch + angleRad;
    
    // Limit pitch to ±89 degrees
    const float maxPitch = 89.0f * 3.14159265359f / 180.0f;
    if (newPitch > maxPitch) newPitch = maxPitch;
    if (newPitch < -maxPitch) newPitch = -maxPitch;
    
    // Project forward onto horizontal plane for yaw calculation
    float horizontal[3] = {forward[0], 0.0f, forward[2]};
    float horizontalLength = sqrt(horizontal[0] * horizontal[0] + horizontal[2] * horizontal[2]);
    
    if (horizontalLength <= 0.0f) {
        // Looking straight up/down, need to preserve some horizontal direction
        // Use a default direction (1, 0, 0) rotated by current yaw if we had one
        horizontal[0] = 1.0f;
        horizontal[2] = 0.0f;
        horizontalLength = 1.0f;
    }
    
    // Normalize horizontal
    horizontal[0] /= horizontalLength;
    horizontal[2] /= horizontalLength;
    
    // Calculate new forward vector with new pitch
    float cosPitch = cos(newPitch);
    float sinPitch = sin(newPitch);
    
    float newForward[3] = {
        horizontal[0] * cosPitch * length,
        sinPitch * length,
        horizontal[2] * cosPitch * length
    };
    
    // Update target
    m_target[0] = m_position[0] + newForward[0];
    m_target[1] = m_position[1] + newForward[1];
    m_target[2] = m_position[2] + newForward[2];
    
    calculateViewMatrix();
}

