#include <scene/ecs/Registry.h>

Entity::Entity(EntityRegistry* parent)
    : m_parent(parent)
{
    m_children = createShared<EntityRegistry>(this);
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

    if (current->getParent() == nullptr)
    {
        return;
    }

    path.push_back(current);

    recurseAbsolutePath(current->getParent()->owned_by(), path);
}