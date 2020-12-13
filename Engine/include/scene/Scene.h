#pragma once

#include <vector>

#include <scene/ecs/Registry.h>
#include <scene/EditorCamera.h>

class Entity;

class Scene
{
public:
    Scene();
    ~Scene();
    
    void onUpdateEditor(float dt, EditorCamera& camera);
    void onViewportResize(uint32_t width, uint32_t height);

    Entity createEntity();
    void destroyEntity(Entity entity);

    EntityRegistry& getRegistry()
    {
        return m_registry;
    }

    Entity* getPrimaryCameraEntity();
    
    template<typename T>
    void onComponentAdded(Entity* entity, T& component);

private:
    EntityRegistry m_registry;

    uint32_t m_viewportWidth = 0, m_viewportHeight = 0;
};