#pragma once

#include <vector>
#include <unordered_map>
#include <algorithm>
#include <typeindex>
#include <typeinfo>

#include <scene/GameComponent.h>
#include <core/Logger.h>

namespace Engine
{

class GameObject
{
public:
    GameObject(GameObject* parent = nullptr)
        : m_parent(parent)
    {
        m_components.clear();
    }

    ~GameObject()
    {
        
    }

    void destroy()
    {
        if (m_parent)
        {
            m_parent->removeChild(this);

            for (auto& component : m_components)
            {
                delete component.second;
            }
        }
    }

    GameObject* addChild()
    {
        m_children.push_back(GameObject(this));
        return &m_children.back();
    }

    void removeChild(GameObject* child)
    {
        for (size_t i = 0; i < m_children.size(); i++)
        {
            if (&m_children[i] == child)
            {
                m_children.erase(m_children.begin() + i);// TODO: Recurse the children as well
                return;
            }
        }
    }

    std::vector<GameObject>& getChildren()
    {
        return m_children;
    }

    template<typename T, typename... Args>
    T& addComponent(Args&& ...args)
    {
        T* component = new T(std::forward<Args>(args)...);
        this->addExistingComponent<T>(component);
        return *component;
    }

    // Component is move'd in (just in case it would otherwise be a heavy copy)
    template<typename T>
    void addExistingComponent(GameComponent* component)
    {
        if (std::is_base_of<GameComponent, T>())
        {
            component->setOwner(this);
            m_components.emplace(std::make_pair(static_cast<std::type_index>(typeid(T)), component));
        }
    }

    template<typename T>
    std::vector<GameObject*> getChildrenWithComponent()
    {
        std::vector<GameObject*> objects;

        for (auto& child : m_children)
        {
            if (child.hasComponent<T>())
            {
                objects.push_back(&child);
            }
        }

        return objects;
    }

    template<typename... Components>
    std::vector<GameObject*> getChildrenWithComponents()
    {
        std::vector<GameObject*> objects;

        for (auto& child : m_children)
        {
            bool check = true;
            (multiComponentCheck<Components>(child, check), ...);

            if (check)
            {
                objects.push_back(&child);
            }
        }

        return objects;
    }

    template<typename T>
    void removeComponent()
    {
        for (auto& component : m_components)
        {
            if (static_cast<std::type_index>(typeid(T)) == component.first)
            {
                m_components.erase(component.first);
            }
        }
    }

    template<typename T>
    T& getComponent()
    {
        if (std::is_base_of<GameComponent, T>())
        {
            return static_cast<T&>(*m_components.at(typeid(T)));
        }

        Logger::getCoreLogger()->critical("Component is not derived from class GameComponent!");
        abort();
    }

    template<typename T>
    bool hasComponent()
    {
        if (std::is_base_of<GameComponent, T>())
        {
            for (auto& component : m_components)
            {
                if (component.first == static_cast<std::type_index>(typeid(T)))
                {
                    return true;
                }
            }
        }

        return false;
    }

    template<typename... Components>
    bool hasComponents()
    {
        bool check = true;
        (multiComponentCheck<Components>(*this, check), ...);
        return check;
    }

private:
    std::unordered_map<std::type_index, GameComponent*> m_components;
    std::vector<GameObject> m_children;
    GameObject* m_parent = nullptr;

    template<typename T>
    void multiComponentCheck(GameObject& object, bool& check)
    {
        if (!check)
        {
            return; // Every component must be present
        }

        check = object.hasComponent<T>();
    }
};

}