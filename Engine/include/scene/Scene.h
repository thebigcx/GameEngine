#pragma once

#include <vector>

#include <scene/ecs/Registry.h>

class Entity;

class Scene
{
public:
    Scene();
    ~Scene();
    
    void onUpdate();
    void onViewportResize(uint32_t width, uint32_t height);

    Entity createEntity();
    void destroyEntity(Entity entity);

    EntityRegistry& getRegistry()
    {
        return m_registry;
    }

private:
    EntityRegistry m_registry;
};