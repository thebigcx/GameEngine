#pragma once

/*#include <unordered_map>
#include <typeindex>

#include <core/Logger.h>

class IComponent
{
public:
    virtual ~IComponent() = default;
};

template<typename T>
class Component : public IComponent
{
public:
    template<typename... Args>
    Component(Args... args) 
    : value(args...) {};

    T value;
};

class EntityRegistry;

class Entity
{
private:
    friend class EntityRegistry;
    friend class EntityView;

public:
    Entity(EntityRegistry* registry)
        : m_registry(registry) {}
    ~Entity()
    {
        
    }



    EntityRegistry* getRegistry()
    {
        return m_registry;
    }

private:
    EntityRegistry* m_registry;

    std::unordered_map<std::type_index, IComponent*> m_components;
};*/