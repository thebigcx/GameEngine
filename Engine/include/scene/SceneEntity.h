#pragma once

#include <scene/ecs/Entity.h>
#include <scene/Scene.h>

class SceneEntity
{
public:
    SceneEntity() = default;
    SceneEntity(Entity* entity, Scene* scene);
    SceneEntity(const SceneEntity& other) = default;

    template<typename T, typename... Args>
    T& addComponent(Args... args)
    {
        T& component = m_scene->getRegistry().emplace<T>(m_entityHandle, args...);
        m_scene->onComponentAdded<T>(*this, component);
        return component;
    }

    template<typename T>
    T& getComponent()
    {
        return m_scene->getRegistry().get<T>(m_entityHandle);
    }

    template<typename T>
    bool hasComponent()
    {
        return m_scene->getRegistry().has<T>(m_entityHandle);
    }

    template<typename T>
    void removeComponent()
    {
        m_scene->getRegistry().remove<T>(m_entityHandle);
    }

    bool operator==(const SceneEntity& other)
    {
        return other.m_entityHandle == m_entityHandle && other.m_scene == m_scene;
    }

    bool operator!=(const SceneEntity& other)
    {
        return !(*this == other);
    }

    operator bool() const { return m_entityHandle != nullptr; }

private:
    Scene* m_scene;
    Entity* m_entityHandle = nullptr;

    friend class Scene;
};