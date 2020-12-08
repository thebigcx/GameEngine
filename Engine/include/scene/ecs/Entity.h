#pragma once

#include <unordered_map>
#include <typeindex>

#include <scene/ecs/Registry.h>
#include <scene/ecs/Component.h>
#include <core/Logger.h>

class Entity
{
public:
    Entity(EntityRegistry* registry);

    template<typename T, typename... Args>
    void addComponent(Args... args)
    {
        if (m_components.find(typeid(T)) == m_components.end())
        {
            Logger::getCoreLogger()->error("Entity already has specified component.");
        }
        else
        {
            m_components.insert(std::pair<std::type_index, Component*>(typeid(T), new T(std::forward(args...))));
        }
    }

    template<typename T>
    T& getComponent()
    {
        return *(m_components.at(typeid(T)));
    }

    template<typename T>
    bool hasComponent()
    {
        return m_components.find(typeid(T)) != m_components.end();
    }

    template<typename T>
    void removeComponent()
    {
        m_components.erase(typeid(T));
    }

private:
    EntityRegistry* m_registry;

    std::unordered_map<std::type_index, Component*> m_components;
};