#include <scene/ecs/Registry.h>

namespace Ecs
{

Entity::Entity(Registry* parent)
    : m_parent(parent)
{
    m_children = createShared<Registry>(this);
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

}