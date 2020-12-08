#pragma once

#include <vector>

class Entity;

class EntityRegistry
{
public:
    EntityRegistry();

    Entity* create();

private:
    std::vector<Entity> m_entities;
};