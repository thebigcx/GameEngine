#pragma once

namespace Engine
{

class GameObject;

class GameComponent
{
public:
    virtual ~GameComponent() = default;

    void setOwner(GameObject* owner)
    {
        m_owner = owner;
    }

protected:
    GameObject* m_owner = nullptr;
};

}