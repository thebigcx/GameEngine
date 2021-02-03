#pragma once

namespace Engine
{

class Transform;
class GameObject;

class GameComponent
{
public:
    virtual ~GameComponent() = default;

    virtual void onTransformChange(const Transform& transform) {}

    void setOwner(GameObject* owner)
    {
        m_owner = owner;
    }

protected:
    GameObject* m_owner = nullptr;
};

}