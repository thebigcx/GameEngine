#pragma once

#include <scene/ecs/Entity.h>

class ScriptableEntity
{
public:
    virtual ~ScriptableEntity() = default;

    template<typename T>
    T& getComponent()
    {
        return m_entity->getRegistry()->get<T>(m_entity);
    }

protected:
    virtual void onCreate() {}
    virtual void onDestroy() {}
    virtual void onUpdate(float dt) {}

private:
    Entity* m_entity;
    friend class Scene;
};