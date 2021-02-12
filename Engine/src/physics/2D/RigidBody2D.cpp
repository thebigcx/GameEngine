#include <physics/2D/RigidBody2D.h>
#include <physics/2D/Collider2D.h>
#include <scene/Scene.h>
#include <util/Transform.h>

namespace Engine
{

RigidBody2D::RigidBody2D()
{
    
}

RigidBody2D::~RigidBody2D()
{
    releaseBody();
    m_world->removeRigidBody(this);
}

void RigidBody2D::onSceneSet(const Reference<Scene>& scene)
{
    m_world = scene->getPhysicsWorld2D()->getComponent<PhysicsWorld2D>();
    m_world->addRigidBody(this);
}

void RigidBody2D::createBody()
{
    math::vec3 position = m_owner->getComponent<Transform>()->getWorldTranslation();
    float rotation = m_owner->getComponent<Transform>()->getWorldRotation().z;

    m_def.position = b2Vec2{position.x, position.y};
    m_def.angle = math::radians(rotation);

    m_body = m_world->getWorld().CreateBody(&m_def);
}

void RigidBody2D::releaseBody()
{
    m_world->getWorld().DestroyBody(m_body);
    m_body = nullptr;
}

void RigidBody2D::addCollider(Collider2D* collider)
{

}

void RigidBody2D::removeCollider(Collider2D* collider)
{

}

}