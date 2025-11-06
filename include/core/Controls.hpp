#ifndef CONTROLS_HPP
#define CONTROLS_HPP

#include "window/Window.hpp"
#include "core/Camera.hpp"

class Controls {
public:
    Controls(Window& window, Camera& camera);
    
    void update(float deltaTime = 0.016f); // Default to ~60fps
    
    // Configuration
    void setMovementSpeed(float speed) { m_movementSpeed = speed; }
    void setRotationSpeed(float speed) { m_rotationSpeed = speed; }
    
private:
    Window& m_window;
    Camera& m_camera;
    float m_movementSpeed;
    float m_rotationSpeed;
    
    bool isKeyPressed(int key);
};

#endif // CONTROLS_HPP

