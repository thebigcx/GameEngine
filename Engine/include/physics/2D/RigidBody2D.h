#pragma once

#include <scene/GameComponent.h>
#include <physics/2D/PhysicsWorld2D.h>

#include <box2d/box2d.h>

namespace Engine
{

class Collider2D;

class RigidBody2D : public GameComponent
{
public:
    RigidBody2D();
    ~RigidBody2D();

    void onSceneSet(const Reference<Scene>& scene) override;

    void createBody();
    void releaseBody();

    void addCollider(Collider2D* collider);
    void removeCollider(Collider2D* collider);

private:
    PhysicsWorld2D* m_world = nullptr;

    b2BodyDef m_def;
    b2Body* m_body = nullptr;
};

}