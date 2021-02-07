#pragma once

#include <vector>
#include <unordered_map>
#include <algorithm>
#include <typeindex>
#include <typeinfo>

#include <scene/GameComponent.h>
#include <core/Logger.h>
#include <util/Transform.h>

namespace Engine
{

// The whole system is based on a hierarchy. A root game object contains children that hold components (and other children).
class GameObject
{
public:
    GameObject(GameObject* parent = nullptr)
        : m_parent(parent)
    {
        m_components.clear();
    }

    ~GameObject() = default;

    GameObject* getParent() const
    {
        return m_parent;
    }

    void onTransformChange(const Transform& transform)
    {
        for (auto& child : m_children)
        {
            child.onTransformChange(transform);
        }
        /*
        for (auto& component : m_components)
        {
            if (component.first == typeid(Transform) && static_cast<Transform*>(component.second) == &transform)
            {
                continue;
            }
                
            component.second->onTransformChange(transform);
        }*/
        if (&getComponent<Transform>() != &transform)
            getComponent<Transform>().onTransformChange(transform);
    }

    void removeChild(GameObject* child)
    {
        child->destroy_();

        for (unsigned int i = 0; i < m_children.size(); i++)
        {
            if (&m_children[i] == child)
            {
                m_children.erase(m_children.begin() + i);
                break;
            }
        }
    }

    GameObject* createChild()
    {
        auto object = GameObject(this);
        object.createComponent<Transform>();
        m_children.push_back(object);
        return &m_children.back();
    }

    std::vector<GameObject>& getChildren()
    {
        return m_children;
    }

    /**
     * Adds a component to the game object. NOTE: When using the return value
     * as 'auto', make sure to take by reference. i.e. auto& comp = obj->createComponent();
     */
    template<typename T, typename... Args>
    T& createComponent(Args&& ...args)
    {
        if (!hasComponent<T>())
        {
            T* component = new T(std::forward<Args>(args)...);
            this->addExistingComponent<T>(component);
            return *component;
        }

        return getComponent<T>();
    }

    template<typename T>
    void addExistingComponent(GameComponent* component)
    {
        if (std::is_base_of<GameComponent, T>() && !hasComponent<T>())
        {
            component->setOwner(this);
            m_components.emplace(std::pair<std::type_index, GameComponent*>(static_cast<std::type_index>(typeid(T)), component));
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
            static_cast<T&>(*m_components.at(typeid(T))).setOwner(this); // TODO: why is this necessary?
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

    //std::vector<GameComponent*> m_listeners;

    template<typename T>
    void multiComponentCheck(GameObject& object, bool& check)
    {
        if (!check)
        {
            return; // Every component must be present
        }

        check = object.hasComponent<T>();
    }

    void destroy_()
    {
        for (auto& component : m_components)
        {
            delete component.second;
        }
        m_components.clear();

        for (auto& child : m_children)
        {
            child.destroy_();
        }

        /*
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
        */
    }
};

}