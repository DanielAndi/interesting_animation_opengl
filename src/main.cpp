#include "window/Window.hpp"
#include <iostream>

int main() {
    // Create window
    Window window(800, 600, "OpenGL Animation");

    // Initialize window
    if (!window.initialize()) {
        std::cerr << "Failed to initialize window" << std::endl;
        return -1;
    }

    // Main loop
    while (!window.shouldClose()) {
        // Clear the screen
        window.clear(0.2f, 0.3f, 0.3f, 1.0f);

        // Swap buffers and poll events
        window.swapBuffers();
        window.pollEvents();
    }

    // Cleanup is handled by Window destructor
    return 0;
}

