#include <scene/ecs/Registry.h>

Entity::Entity(EntityRegistry* parent)
    : m_parent(parent)
{
    m_children = createShared<EntityRegistry>();
}

Entity::~Entity()
{
    
}

void Entity::recurseAbsolutePath(Entity* current, std::vector<Entity*>& path)
{
    if (current == nullptr) // Root
    {
        return;
    }
    path.insert(path.begin(), current);
    recurseAbsolutePath(getParent()->owned_by(), path);
}