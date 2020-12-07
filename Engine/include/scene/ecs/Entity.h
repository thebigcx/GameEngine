#pragma once

#include <scene/Scene.h>

class Entity
{
public:
    Entity(Scene* scene);

    template<typename T, typename... Args>
    void addComponent(Args... args)
    {

    }

    template<typename T>
    T& getComponent()
    {

    }

    template<typename T>
    bool hasComponent()
    {

    }

    template<typename T>
    void removeComponent()
    {
        
    }

private:
    Scene* m_scene;
};