#include "core/Controls.hpp"
#include <GLFW/glfw3.h>

Controls::Controls(Window& window, Camera& camera)
    : m_window(window), m_camera(camera), m_movementSpeed(5.0f), m_rotationSpeed(90.0f) {
}

void Controls::update(float deltaTime) {
    GLFWwindow* glfwWindow = m_window.getHandle();
    if (!glfwWindow) return;
    
    float moveDistance = m_movementSpeed * deltaTime;
    float rotateAngle = m_rotationSpeed * deltaTime;
    
    // Movement controls: WASD
    if (isKeyPressed(GLFW_KEY_W)) {
        m_camera.moveForward(moveDistance);
    }
    if (isKeyPressed(GLFW_KEY_S)) {
        m_camera.moveForward(-moveDistance);
    }
    if (isKeyPressed(GLFW_KEY_A)) {
        m_camera.moveRight(-moveDistance);
    }
    if (isKeyPressed(GLFW_KEY_D)) {
        m_camera.moveRight(moveDistance);
    }
    
    // Vertical movement: Q/E
    if (isKeyPressed(GLFW_KEY_Q)) {
        m_camera.moveUp(-moveDistance);
    }
    if (isKeyPressed(GLFW_KEY_E)) {
        m_camera.moveUp(moveDistance);
    }
    
    // Rotation controls: Arrow keys
    if (isKeyPressed(GLFW_KEY_LEFT)) {
        m_camera.rotateYaw(-rotateAngle);
    }
    if (isKeyPressed(GLFW_KEY_RIGHT)) {
        m_camera.rotateYaw(rotateAngle);
    }
    if (isKeyPressed(GLFW_KEY_UP)) {
        m_camera.rotatePitch(rotateAngle);
    }
    if (isKeyPressed(GLFW_KEY_DOWN)) {
        m_camera.rotatePitch(-rotateAngle);
    }
    
    // Alternative rotation controls: IJKL
    if (isKeyPressed(GLFW_KEY_I)) {
        m_camera.rotatePitch(rotateAngle);
    }
    if (isKeyPressed(GLFW_KEY_K)) {
        m_camera.rotatePitch(-rotateAngle);
    }
    if (isKeyPressed(GLFW_KEY_J)) {
        m_camera.rotateYaw(-rotateAngle);
    }
    if (isKeyPressed(GLFW_KEY_L)) {
        m_camera.rotateYaw(rotateAngle);
    }
}

bool Controls::isKeyPressed(int key) {
    GLFWwindow* glfwWindow = m_window.getHandle();
    if (!glfwWindow) return false;
    return glfwGetKey(glfwWindow, key) == GLFW_PRESS;
}

