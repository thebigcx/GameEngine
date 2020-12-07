#pragma once

#include <vector>

class Entity;

class Scene
{
public:
    Scene();
    ~Scene();
    
    void onUpdate();

    Entity createEntity();
    void destroyEntity(Entity entity);

private:
    //std::vector<Entity> m_entities;
};