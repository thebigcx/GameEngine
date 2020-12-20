#include <scene/ecs/Registry.h>

Entity::Entity(EntityRegistry* parent)
    : m_parent(parent)
{
    m_children = createShared<EntityRegistry>();
}

Entity::~Entity()
{
    
}