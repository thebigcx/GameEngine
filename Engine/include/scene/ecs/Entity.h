#pragma once

#include <unordered_map>
#include <typeindex>

#include <scene/ecs/Component.h>
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
    Component() = default;

    T value;
};

class EntityRegistry;

class Entity
{
public:
    Entity(EntityRegistry* registry);
    ~Entity()
    {
        for (auto& component : m_components)
        {
            delete component.second;
        }
    }

    template<typename T, typename... Args>
    void addComponent(Args... args)
    {
        if (this->hasComponent<T>())
        {
            Logger::getCoreLogger()->error("Entity already has specified component.");
        }
        else
        {
            m_components.insert(std::pair<std::type_index, IComponent*>(typeid(T), new Component<T>()));
        }
    }

    template<typename T>
    void addComponent()
    {
        if (this->hasComponent<T>())
        {
            Logger::getCoreLogger()->error("Entity already has specified component.");
        }
        else
        {
            m_components.insert(std::pair<std::type_index, IComponent*>(typeid(T), new Component<T>()));
        }
    }

    template<typename T>
    T& getComponent()
    {
        if (!this->hasComponent<T>())
        {
            Logger::getCoreLogger()->error("Entity does not have specified component: %s", typeid(T).name());
        }

        return static_cast<Component<T>&>(*(m_components.at(typeid(T)))).value;
    }

    template<typename T>
    bool hasComponent()
    {
        return m_components.find(typeid(T)) != m_components.end();
    }

    template<typename T>
    void removeComponent()
    {
        if (!this->hasComponent<T>())
        {
            Logger::getCoreLogger()->error("Entity does not have specified component: %s", typeid(T).name());
        }

        m_components.erase(typeid(T));
    }

private:
    EntityRegistry* m_registry;

    std::unordered_map<std::type_index, IComponent*> m_components;
};