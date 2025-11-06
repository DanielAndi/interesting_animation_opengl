#ifndef SCENE_HPP
#define SCENE_HPP

#include "scene/SceneObject.hpp"
#include "core/Camera.hpp"
#include "core/Shader.hpp"
#include <vector>
#include <memory>

class Scene {
public:
    Scene(float width, float height);
    ~Scene();

    bool initialize();
    void render();
    void cleanup();
    
    void addObject(const std::string& modelPath, float posX = 0.0f, float posY = 0.0f, float posZ = 0.0f,
                  float scaleX = 1.0f, float scaleY = 1.0f, float scaleZ = 1.0f);
    
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

