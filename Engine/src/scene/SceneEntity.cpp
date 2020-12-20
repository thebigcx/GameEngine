#include <scene/SceneEntity.h>
#include <scene/Components.h>

SceneEntity::SceneEntity(Entity* entity, Scene* scene)
    : m_entityHandle(entity), m_scene(scene)
{
    
}

SceneEntity SceneEntity::createNull(Scene* scene)
{
    return SceneEntity(nullptr, scene);
}

void SceneEntity::addChild(const std::string& name)
{
    auto entity = m_entityHandle->getChildren()->create();
    entity->getParent()->emplace<TagComponent>(entity, name);
}