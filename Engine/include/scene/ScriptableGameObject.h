#pragma once

#include <scene/GameObject.h>

namespace Engine
{

class ScriptableGameObject
{
public:
    virtual ~ScriptableGameObject() = default;

    template<typename T>
    T& getComponent()
    {
        return m_gameObject->getComponent<T>();
    }

protected:
    virtual void onCreate() {}
    virtual void onDestroy() {}
    virtual void onUpdate(float dt) {}
    virtual void onCollide2D() {}

private:
    GameObject* m_gameObject;
    friend class Scene;
};

}