#include "scene/Scene.hpp"
#include <iostream>
#include <algorithm>

Scene::Scene(float width, float height) 
    : m_camera(width, height), m_width(width), m_height(height) {
}

Scene::~Scene() {
    cleanup();
}

bool Scene::initialize() {
    if (!loadShaders()) {
        return false;
    }
    
    setupCamera();
    
    return true;
}

bool Scene::loadShaders() {
    // Create shader source code
    const std::string vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;

void main() {
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoord = aTexCoord;
    gl_Position = projection * view * vec4(FragPos, 1.0);
}
)";

    const std::string fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D texture_diffuse1;
uniform bool useTexture;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;

void main() {
    vec3 objectColor;
    if (useTexture) {
        objectColor = texture(texture_diffuse1, TexCoord).rgb;
    } else {
        objectColor = vec3(0.7, 0.7, 0.7);
    }
    
    // Ambient
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * lightColor;
    
    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // Specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;
    
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}
)";

    return m_shader.loadFromSource(vertexShaderSource, fragmentShaderSource);
}

void Scene::setupCamera() {
    if (m_objects.empty()) {
        // Default camera position
        m_camera.setPosition(0.0f, 2.0f, 5.0f);
        m_camera.setTarget(0.0f, 0.0f, 0.0f);
        return;
    }
    
    // Calculate bounding box of all objects
    float minX = 1e9f, minY = 1e9f, minZ = 1e9f;
    float maxX = -1e9f, maxY = -1e9f, maxZ = -1e9f;
    
    for (const auto& obj : m_objects) {
        float objMinX, objMinY, objMinZ, objMaxX, objMaxY, objMaxZ;
        obj->getBoundingBox(objMinX, objMinY, objMinZ, objMaxX, objMaxY, objMaxZ);
        
        minX = std::min(minX, objMinX);
        minY = std::min(minY, objMinY);
        minZ = std::min(minZ, objMinZ);
        maxX = std::max(maxX, objMaxX);
        maxY = std::max(maxY, objMaxY);
        maxZ = std::max(maxZ, objMaxZ);
    }
    
    // Calculate center and size
    float centerX = (minX + maxX) * 0.5f;
    float centerY = (minY + maxY) * 0.5f;
    float centerZ = (minZ + maxZ) * 0.5f;
    
    float sizeX = maxX - minX;
    float sizeY = maxY - minY;
    float sizeZ = maxZ - minZ;
    float maxSize = std::max({sizeX, sizeY, sizeZ});
    
    std::cout << "Model bounding box: (" << minX << ", " << minY << ", " << minZ 
              << ") to (" << maxX << ", " << maxY << ", " << maxZ << ")" << std::endl;
    std::cout << "Model size: " << sizeX << " x " << sizeY << " x " << sizeZ << std::endl;
    std::cout << "Model center: (" << centerX << ", " << centerY << ", " << centerZ << ")" << std::endl;
    
    // Position camera close to the model, looking at its center
    // Use a distance based on model size, but keep it close
    float distance = std::max(maxSize * 0.8f, 3.0f);
    // Position camera at a slight angle above and to the side for better view
    m_camera.setPosition(centerX + distance * 0.4f, centerY + distance * 0.5f, centerZ + distance * 0.7f);
    m_camera.setTarget(centerX, centerY, centerZ);
    m_camera.update();
    
    std::cout << "Camera position: (" << m_camera.getPositionX() << ", " 
              << m_camera.getPositionY() << ", " << m_camera.getPositionZ() << ")" << std::endl;
}

void Scene::addObject(const std::string& modelPath, float posX, float posY, float posZ,
                     float scaleX, float scaleY, float scaleZ) {
    auto obj = std::make_unique<SceneObject>(modelPath);
    obj->setScale(scaleX, scaleY, scaleZ);
    
    if (obj->load()) {
        // Get the model's raw bounding box (before transformations)
        float minX, minY, minZ, maxX, maxY, maxZ;
        obj->getLocalBoundingBox(minX, minY, minZ, maxX, maxY, maxZ);
        
        // Calculate local center (raw model space)
        float localCenterX = (minX + maxX) * 0.5f;
        float localCenterY = (minY + maxY) * 0.5f;
        float localCenterZ = (minZ + maxZ) * 0.5f;
        
        // Set position to offset the model so its center is at the desired position
        // After scaling, the center will be at localCenter * scale, so we offset by that
        obj->setPosition(posX - localCenterX * scaleX, 
                        posY - localCenterY * scaleY, 
                        posZ - localCenterZ * scaleZ);
        
        m_objects.push_back(std::move(obj));
        // Update camera after adding object
        setupCamera();
    } else {
        std::cerr << "Failed to load object: " << modelPath << std::endl;
    }
}

void Scene::render() {
    m_shader.use();
    
    // Set view and projection matrices
    m_shader.setMat4("view", m_camera.getViewMatrix());
    m_shader.setMat4("projection", m_camera.getProjectionMatrix());
    
    // Set lighting
    m_shader.setVec3("lightPos", 5.0f, 5.0f, 5.0f);
    m_shader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
    m_shader.setVec3("viewPos", m_camera.getPositionX(), m_camera.getPositionY(), m_camera.getPositionZ());
    
    // Set texture unit
    m_shader.setInt("texture_diffuse1", 0);
    
    // Render all objects
    for (const auto& obj : m_objects) {
        float modelMatrix[16];
        obj->getModelMatrix(modelMatrix);
        m_shader.setMat4("model", modelMatrix);
        
        // Set useTexture uniform based on whether model has texture
        m_shader.setBool("useTexture", obj->hasTexture());
        
        obj->render();
    }
}

void Scene::cleanup() {
    m_objects.clear();
}

