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

class Transform;

// The whole system is based on a hierarchy. A root game object contains children that hold components (and other children).
class GameObject
{
public:
    GameObject(GameObject* parent = nullptr);
    ~GameObject();

    bool operator==(const GameObject& obj)
    {
        return &obj == this;
    }

    GameObject* getParent() const;

    void onTransformChange(const Transform& transform);

    void removeChild(GameObject* child);
    GameObject* createChild();

    std::vector<GameObject*> getChildren();
    std::vector<GameObject*> getChildrenRecursive();

    template<typename T>
    std::vector<GameObject*> getChildrenWithComponentRecursive()
    {
        std::vector<GameObject*> children;

        for (auto& child : getChildrenRecursive())
        {
            if (child->hasComponent<T>())
            {
                children.push_back(child);
            }
        }

        return children;
    }

    template<typename... Ts>
    std::vector<GameObject*> getChildrenWithComponentsRecursive()
    {
        std::vector<GameObject*> children;

        for (auto& child : getChildrenRecursive())
        {
            if (child->hasComponents<Ts...>())
            {
                children.push_back(child);
            }
        }

        return children;
    } 

    template<typename T, typename... Args>
    T* createComponent(Args&& ...args)
    {
        if (!hasComponent<T>())
        {
            T* component = new T(std::forward<Args>(args)...);
            this->addExistingComponent<T>(component);
            return component;
        }

        return getComponent<T>();
    }

    template<typename T>
    void addExistingComponent(GameComponent* component)
    {
        if (std::is_base_of<GameComponent, T>() && !hasComponent<T>())
        {
            component->setOwner(this);
            m_components.emplace(static_cast<std::type_index>(typeid(T)), component);
        }
    }

    template<typename T>
    std::vector<GameObject*> getChildrenWithComponent()
    {
        std::vector<GameObject*> objects;

        for (auto& child : m_children)
        {
            if (child->hasComponent<T>())
            {
                objects.push_back(child);
            }
        }

        return objects;
    }

    template<typename... Components>
    std::vector<GameObject*> getChildrenWithComponents()
    {
        std::vector<GameObject*> objects;

        for (auto child : m_children)
        {
            bool check = true;
            (multiComponentCheck<Components>(child, check), ...);

            if (check)
            {
                objects.push_back(child);
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
    T* getComponent()
    {
        if (std::is_base_of<GameComponent, T>())
        {
            //reinterpret_cast<T*>(m_components.at(typeid(T)))->setOwner(this); // TODO: why is this necessary?
            return static_cast<T*>(m_components.at(typeid(T)));
        }

        Logger::getCoreLogger()->critical("Component is not derived from class GameComponent!");
        return nullptr;
    }

    template<typename T>
    bool hasComponent()
    {
        return m_components.find(static_cast<std::type_index>(typeid(T))) != m_components.end();
    }

    template<typename... Components>
    bool hasComponents()
    {
        bool check = true;
        (multiComponentCheck<Components>(this, check), ...);
        return check;
    }

private:
    std::unordered_map<std::type_index, GameComponent*> m_components;
    std::vector<GameObject*> m_children;
    GameObject* m_parent = nullptr;

    //std::vector<GameComponent*> m_listeners;

    template<typename T>
    void multiComponentCheck(GameObject* object, bool& check)
    {
        check = check ? object->hasComponent<T>() : false;
    }

    void destroy_();
};

}