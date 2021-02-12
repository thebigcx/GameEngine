#include <physics/2D/Collider2D.h>
#include <physics/2D/RigidBody2D.h>
#include <scene/GameObject.h>

namespace Engine
{

Collider2D::Collider2D()
{
    
}

Collider2D::~Collider2D()
{
    m_body->removeCollider(this);

    releaseFixture();
}

void Collider2D::createFixture()
{

}

void Collider2D::releaseFixture()
{

}

void Collider2D::onSceneSet(const Reference<Scene>& scene)
{
    createFixture();

    m_body = m_owner->getComponent<RigidBody2D>();
    m_body->addCollider(this);
}

}