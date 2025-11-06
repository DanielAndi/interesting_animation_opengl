#ifndef SCENE_HPP
#define SCENE_HPP

#include "scene/SceneObject.hpp"
#include "core/Camera.hpp"
#include "core/Shader.hpp"
#include <vector>
#include <memory>

/**
 * @class Scene
 * @brief Manages the 3D scene including objects, camera, and rendering.
 * 
 * This class serves as the main container for all scene objects, manages
 * the camera, shaders, and coordinates rendering of the entire scene.
 */
class Scene {
public:
    /**
     * @brief Constructs a scene with the specified viewport dimensions.
     * @param width The width of the viewport in pixels.
     * @param height The height of the viewport in pixels.
     */
    Scene(float width, float height);
    
    /**
     * @brief Destructor that cleans up scene resources.
     */
    ~Scene();

    /**
     * @brief Initializes the scene, loading shaders and setting up the camera.
     * @return True if initialization succeeded, false otherwise.
     */
    bool initialize();
    
    /**
     * @brief Renders all objects in the scene.
     */
    void render();
    
    /**
     * @brief Cleans up all scene resources.
     */
    void cleanup();
    
    /**
     * @brief Adds a 3D object to the scene.
     * @param modelPath Path to the OBJ model file.
     * @param posX X position in world space (default: 0.0).
     * @param posY Y position in world space (default: 0.0).
     * @param posZ Z position in world space (default: 0.0).
     * @param scaleX X scale factor (default: 1.0).
     * @param scaleY Y scale factor (default: 1.0).
     * @param scaleZ Z scale factor (default: 1.0).
     */
    void addObject(const std::string& modelPath, float posX = 0.0f, float posY = 0.0f, float posZ = 0.0f,
                  float scaleX = 1.0f, float scaleY = 1.0f, float scaleZ = 1.0f);
    
    /**
     * @brief Gets a reference to the scene's camera.
     * @return Reference to the Camera object.
     */
    Camera& getCamera() { return m_camera; }

private:
    std::vector<std::unique_ptr<SceneObject>> m_objects;
    Camera m_camera;
    Shader m_shader;
    float m_width;
    float m_height;
    
    void setupCamera();
    bool loadShaders();
};

#endif // SCENE_HPP

