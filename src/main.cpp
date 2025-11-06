#include "window/Window.hpp"
#include "scene/Scene.hpp"
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

    // Main loop
    while (!window.shouldClose()) {
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

