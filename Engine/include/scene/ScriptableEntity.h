#pragma once

#include <scene/SceneEntity.h>

class ScriptableEntity
{
public:
    virtual ~ScriptableEntity() = default;

    template<typename T>
    T& getComponent()
    {
        return m_entity.getComponent<T>();
    }

protected:
    virtual void onCreate() {}
    virtual void onDestroy() {}
    virtual void onUpdate(float dt) {}

private:
    SceneEntity m_entity;
    friend class Scene;
};