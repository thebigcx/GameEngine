#pragma once

#include <scene/GameObject.h>

namespace Engine
{

class ScriptableObject
{
    friend class Scene;

public:
    virtual ~ScriptableObject() = default;

    template<typename T>
    T* getComponent()
    {
        return m_gameObject->getComponent<T>();
    }

protected:
    virtual void onStart() {}
    virtual void onDestroy() {}
    virtual void onUpdate(float dt) {}

private:
    GameObject* m_gameObject;
};

}