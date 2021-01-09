#pragma once

#include <physics/Body.h>
#include <physics/Collider.h>

namespace Engine
{

class BoxBody : public Body
{
public:
    BoxBody()
    {
        m_collider = new BoxCollider(this);
    }

    math::vec2 size;
};

}