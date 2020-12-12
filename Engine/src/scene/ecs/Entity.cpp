#include <scene/ecs/Entity.h>
#include <scene/ecs/Registry.h>

Entity::Entity(EntityRegistry* registry)
    : m_registry(registry)
{

}