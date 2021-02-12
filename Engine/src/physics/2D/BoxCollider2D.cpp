#include <physics/2D/BoxCollider2D.h>

namespace Engine
{

BoxCollider2D::BoxCollider2D()
{
    m_shape.SetAsBox(100, 100);

    
}

BoxCollider2D::~BoxCollider2D()
{

}

}