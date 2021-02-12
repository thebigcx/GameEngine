#pragma once

#include <core/Core.h>

namespace Engine
{

class Transform;
class GameObject;
class Scene;

class GameComponent
{
public:
    virtual ~GameComponent() = default;

    virtual void onSceneSet(const Reference<Scene>& scene) {}
    virtual void onGameObjectSet(GameObject* object) {}
    virtual void onTransformChange(const Transform& transform) {}

    void setOwner(GameObject* owner)
    {
        m_owner = owner;
        //onGameObjectSet(owner);
    }

protected:
    GameObject* m_owner = nullptr;
};

}