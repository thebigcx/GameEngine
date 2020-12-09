#include <scene/ecs/Registry.h>
#include <scene/ecs/Entity.h>

EntityRegistry::EntityRegistry()
{

}

Entity* EntityRegistry::create()
{
    m_entities.push_back(Entity(this));
    return &(m_entities[m_entities.size() - 1]);
}

void EntityRegistry::destroy(Entity* entity)
{

}

void EntityRegistry::each(const std::function<void(Entity* entity)>& fn)
{
    for (auto& entity : m_entities)
    {
        fn(&entity);
    }
}