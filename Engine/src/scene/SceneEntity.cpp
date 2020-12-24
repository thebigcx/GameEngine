#include <scene/SceneEntity.h>
#include <scene/Components.h>

namespace Engine
{

SceneEntity::SceneEntity(Ecs::Entity* entity, Scene* scene)
    : m_entityHandle(entity), m_scene(scene)
{
    
}

SceneEntity SceneEntity::createNull(Scene* scene)
{
    return SceneEntity(nullptr, scene);
}

Ecs::Entity* SceneEntity::addChild(const std::string& name)
{
    auto entity = m_entityHandle->getChildren()->create();
    entity->getParent()->emplace<TagComponent>(entity, name);
    return entity;
}

}