#ifndef CONTROLS_HPP
#define CONTROLS_HPP

#include "window/Window.hpp"
#include "core/Camera.hpp"

/**
 * @class Controls
 * @brief Handles user input and translates it into camera movement and rotation.
 * 
 * This class processes keyboard and mouse input from the window and updates
 * the camera accordingly. It provides configurable movement and rotation speeds.
 */
class Controls {
public:
    /**
     * @brief Constructs a Controls object that manages input for the given camera.
     * @param window Reference to the window for input polling.
     * @param camera Reference to the camera to control.
     */
    Controls(Window& window, Camera& camera);
    
    /**
     * @brief Updates the controls based on current input state.
     * @param deltaTime Time elapsed since last update in seconds (default ~60fps).
     */
    void update(float deltaTime = 0.016f); // Default to ~60fps
    
    /**
     * @brief Sets the movement speed for camera translation.
     * @param speed The movement speed in units per second.
     */
    void setMovementSpeed(float speed) { m_movementSpeed = speed; }
    
    /**
     * @brief Sets the rotation speed for camera rotation.
     * @param speed The rotation speed in radians per second.
     */
    void setRotationSpeed(float speed) { m_rotationSpeed = speed; }
    
private:
    Window& m_window;
    Camera& m_camera;
    float m_movementSpeed;
    float m_rotationSpeed;
    
    bool isKeyPressed(int key);
};

#endif // CONTROLS_HPP

