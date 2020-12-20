#include <scene/SceneEntity.h>

SceneEntity::SceneEntity(Entity* entity, Scene* scene)
    : m_entityHandle(entity), m_scene(scene)
{
    
}

SceneEntity SceneEntity::createNull(Scene* scene)
{
    return SceneEntity(nullptr, scene);
}