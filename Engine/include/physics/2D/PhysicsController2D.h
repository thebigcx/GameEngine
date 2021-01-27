#pragma once

#include <box2d/b2_world.h>

#include <scene/Scene.h>

namespace Engine
{

class PhysicsController2D
{
public:
    ~PhysicsController2D();

public:
    void initialize();
    void finalize();

    void onUpdate(float dt);

    inline void setScene(Scene& scene) { m_scene = &scene; }

    static PhysicsController2D* getInstance();

private:
    PhysicsController2D();

    Scene* m_scene = nullptr;

    b2World* m_world = nullptr;
};

}