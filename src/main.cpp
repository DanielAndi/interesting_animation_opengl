#include "window/Window.hpp"
#include "scene/Scene.hpp"
#include "core/Controls.hpp"
#include <GLFW/glfw3.h>
#include <iostream>

int main() {
    // Create window
    Window window(800, 600, "OpenGL Animation");

    // Initialize window
    if (!window.initialize()) {
        std::cerr << "Failed to initialize window" << std::endl;
        return -1;
    }

    // Create scene
    Scene scene(window.getWidth(), window.getHeight());
    
    // Initialize scene
    if (!scene.initialize()) {
        std::cerr << "Failed to initialize scene" << std::endl;
        return -1;
    }

    // Add mountain model to scene at origin (0,0,0)
    scene.addObject("models/mountain/mount.blend1.obj", 0.0f, 0.0f, 0.0f);

    // Create controls for camera
    Controls controls(window, scene.getCamera());
    
    // Time tracking for deltaTime calculation
    double lastTime = glfwGetTime();

    // Main loop
    while (!window.shouldClose()) {
        // Calculate deltaTime
        double currentTime = glfwGetTime();
        float deltaTime = static_cast<float>(currentTime - lastTime);
        lastTime = currentTime;
        
        // Update controls (camera movement and rotation)
        controls.update(deltaTime);
        
        // Clear the screen
        window.clear(0.2f, 0.3f, 0.3f, 1.0f);

        // Render scene
        scene.render();

        // Swap buffers and poll events
        window.swapBuffers();
        window.pollEvents();
    }

    // Cleanup is handled by destructors
    return 0;
}

