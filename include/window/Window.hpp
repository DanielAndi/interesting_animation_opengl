#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

class Window {
public:
    Window(int width, int height, const std::string& title);
    ~Window();

    bool initialize();
    void destroy();
    bool shouldClose() const;
    void swapBuffers();
    void pollEvents();
    void clear(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f);

    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }
    GLFWwindow* getHandle() const { return m_window; }

private:
    int m_width;
    int m_height;
    std::string m_title;
    GLFWwindow* m_window;
};

#endif // WINDOW_HPP

