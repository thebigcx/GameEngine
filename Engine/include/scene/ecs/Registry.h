#pragma once

#include <vector>
#include <functional>

#include <scene/ecs/Entity.h>

class EntityRegistry
{
public:
    EntityRegistry();

    Entity* create();
    void destroy(Entity* entity);

    template<typename T>
    void emplace()
    {

    }

    void each(const std::function<void(Entity* entity)>& fn);

private:
    std::vector<Entity> m_entities;
};