#pragma once

#include <vector>

#include <scene/ecs/Registry.h>
#include <scene/EditorCamera.h>
#include <renderer/Material.h>
#include <renderer/Lights.h>

namespace Engine
{

class SceneEntity;

class Scene
{
public:
    Scene();
    ~Scene();
    
    void onUpdateEditor(float dt, EditorCamera& camera);
    void onUpdateRuntime(float dt);

    void onViewportResize(uint32_t width, uint32_t height);

    SceneEntity createEntity(const std::string& name);
    void destroyEntity(SceneEntity& entity);

    Ecs::Registry& getRegistry()
    {
        return m_registry;
    }

    SceneEntity getPrimaryCameraEntity();
    
    template<typename T>
    void onComponentAdded(SceneEntity& entity, T& component);

private:
    Ecs::Registry m_registry;

    uint32_t m_viewportWidth = 0, m_viewportHeight = 0;

    void render2DEntities();
    void render3DEntities();

    LightSetup m_setup;
};

}