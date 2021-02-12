#pragma once

#include <physics/2D/Collider2D.h>

#include <box2d/box2d.h>

namespace Engine
{

class BoxCollider2D : public Collider2D
{
public:
    BoxCollider2D();
    ~BoxCollider2D();

private:
    b2PolygonShape m_shape;

};

}