#pragma once

#include <scene/GameComponent.h>

#include <box2d/box2d.h>

namespace Engine
{

class RigidBody2D;

class Collider2D : public GameComponent
{
public:
    Collider2D();
    ~Collider2D();

    void onSceneSet(const Reference<Scene>& scene) override;

    void createFixture();
    void releaseFixture();

private:
    RigidBody2D* m_body = nullptr;

    b2FixtureDef m_fixtureDef;
    b2Fixture* m_fixture = nullptr;
};

}