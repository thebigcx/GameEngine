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

    ~GameObject() = default;

    void destroy()
    {
        _destroy();
    }

    void removeChild(GameObject* child)
    {
        child->_destroy();
    }

    GameObject* addChild()
    {
        m_children.push_back(GameObject(this));
        return &m_children.back();
    }

    std::vector<GameObject>& getChildren()
    {
        return m_children;
    }

    /**
     * Adds a component to the game object. NOTE: When using the return value
     * as 'auto', make sure to take by reference. i.e. auto& comp = obj->addComponent();
     */
    template<typename T, typename... Args>
    T& addComponent(Args&& ...args)
    {
        if (!hasComponent<T>())
        {
            T* component = new T(std::forward<Args>(args)...);
            this->addExistingComponent<T>(component);
            return *component;
        }

        Logger::getCoreLogger()->error(std::string("Game Object already has component: ") + typeid(T).name());
        static T fail(std::forward<Args>(args)...);
        return fail;
    }

    // Component is move'd in (just in case it would otherwise be a heavy copy)
    template<typename T>
    void addExistingComponent(GameComponent* component)
    {
        if (std::is_base_of<GameComponent, T>() && !hasComponent<T>())
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
        if (std::is_base_of<GameComponent, T>())
        {
            for (auto& component : m_components)
            {
                if (component.first == static_cast<std::type_index>(typeid(T)))
                {
                    delete component.second;
                    m_components.erase(component.first);
                    
                    break;
                }
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

    void _destroy()
    {
        for (auto& component : m_components)
        {
            delete component.second;
        }

        for (auto& child : m_children)
        {
            child._destroy();
        }

        if (m_parent)
        {
            for (size_t i = 0; i < m_parent->getChildren().size(); i++)
            {
                if (&m_parent->getChildren()[i] == this)
                {
                    m_parent->getChildren().erase(m_parent->getChildren().begin() + i);
                    return;
                }
            }
        }

        m_parent = nullptr;
    }
};

}