#pragma once

#include <scene/Scene.h>

class SceneEntity
{
public:
    SceneEntity() = default;
    SceneEntity(Ecs::Entity* entity, Scene* scene);
    SceneEntity(const SceneEntity& other) = default;

    template<typename T, typename... Args>
    T& addComponent(Args&&... args)
    {
        T& component = m_entityHandle->getParent()->emplace<T>(m_entityHandle, std::forward<Args>(args)...);
        m_scene->onComponentAdded<T>(*this, component);
        return component;
    }

    template<typename T>
    T& getComponent()
    {
        return m_entityHandle->getParent()->get<T>(m_entityHandle);
    }

    template<typename T>
    bool hasComponent()
    {
        if (!m_entityHandle) return false;
        return m_entityHandle->getParent()->has<T>(m_entityHandle);
    }

    template<typename T>
    void removeComponent()
    {
        m_entityHandle->getParent()->remove<T>(m_entityHandle);
    }

    std::vector<SceneEntity> getChildren()
    {
        std::vector<SceneEntity> children;
        m_entityHandle->getChildren()->each([&](Ecs::Entity* entityID)
        {
            children.push_back(SceneEntity(entityID, m_scene));
        });

        return children;
    }

    Ecs::Entity* addChild(const std::string& name);

    const Ecs::Entity* getHandle() const
    {
        return m_entityHandle;
    }

    bool operator==(const SceneEntity& other)
    {
        return other.m_entityHandle == m_entityHandle && other.m_scene == m_scene;
    }

    bool operator!=(const SceneEntity& other)
    {
        return !(*this == other);
    }

    static SceneEntity createNull(Scene* scene);

    operator bool() const { return m_entityHandle != nullptr; }

private:
    Scene* m_scene = nullptr;
    Ecs::Entity* m_entityHandle = nullptr;
    

    friend class Scene;
};