#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

/**
 * @class Window
 * @brief Manages the OpenGL window and rendering context using GLFW.
 * 
 * This class provides a wrapper around GLFW window functionality, handling
 * window creation, event polling, buffer swapping, and cleanup.
 */
class Window {
public:
    /**
     * @brief Constructs a window with the specified dimensions and title.
     * @param width The width of the window in pixels.
     * @param height The height of the window in pixels.
     * @param title The window title.
     */
    Window(int width, int height, const std::string& title);
    
    /**
     * @brief Destructor that destroys the window if it exists.
     */
    ~Window();

    /**
     * @brief Initializes GLFW and creates the OpenGL window.
     * @return True if initialization succeeded, false otherwise.
     */
    bool initialize();
    
    /**
     * @brief Destroys the window and cleans up GLFW resources.
     */
    void destroy();
    
    /**
     * @brief Checks if the window should be closed (e.g., user clicked close button).
     * @return True if the window should close, false otherwise.
     */
    bool shouldClose() const;
    
    /**
     * @brief Swaps the front and back buffers for double buffering.
     */
    void swapBuffers();
    
    /**
     * @brief Polls for window events (input, resize, etc.).
     */
    void pollEvents();
    
    /**
     * @brief Clears the color buffer with the specified color.
     * @param r Red component (0.0 to 1.0, default: 0.0).
     * @param g Green component (0.0 to 1.0, default: 0.0).
     * @param b Blue component (0.0 to 1.0, default: 0.0).
     * @param a Alpha component (0.0 to 1.0, default: 1.0).
     */
    void clear(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f);

    /**
     * @brief Gets the width of the window.
     * @return The window width in pixels.
     */
    int getWidth() const { return m_width; }
    
    /**
     * @brief Gets the height of the window.
     * @return The window height in pixels.
     */
    int getHeight() const { return m_height; }
    
    /**
     * @brief Gets the underlying GLFW window handle.
     * @return Pointer to the GLFWwindow object.
     */
    GLFWwindow* getHandle() const { return m_window; }

private:
    int m_width;
    int m_height;
    std::string m_title;
    GLFWwindow* m_window;
};

#endif // WINDOW_HPP

