#pragma once

#include <vector>
#include <functional>
#include <typeindex>
#include <unordered_map>
#include <typeindex>

#include <core/Logger.h>
#include <core/Core.h>

namespace Engine
{

namespace Ecs
{

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
    Component(Args&&... args) 
        : value(args...) {}

    T value;
};

class Registry;

class Entity
{
private:
    friend class Registry;
    friend class View;

public:
    Entity(Registry* parent);
    ~Entity();

    const Shared<Registry>& getChildren()
    {
        return m_children;
    }

    Registry* getParent()
    {
        return m_parent;
    }

    std::vector<Entity*> getAbsolutePath()
    {
        std::vector<Entity*> path;
        recurseAbsolutePath(this, path);

        if (path.size() != 0)
        {
            path.pop_back(); // remove 'this' as it is not an ancestor
        }
        return path;
    }

private:
    Registry* m_parent;
    Shared<Registry> m_children;

    std::unordered_map<std::type_index, IComponent*> m_components;

    void recurseAbsolutePath(Entity* current, std::vector<Entity*>& path);
};

class View
{
public:
    View(const std::vector<Entity*>& entities)
        : m_entities(entities)
    {

    }

    template<typename T>
    T& get(Entity* pEntity)
    {
        auto it = std::find(m_entities.begin(), m_entities.end(), pEntity);

        if (it == m_entities.end())
        {
            Logger::getCoreLogger()->error("Entity does not have specified component! (Registry::view::get)");
        }

        Entity* entity = m_entities.at(std::distance(m_entities.begin(), it));
        return static_cast<Component<T>&>(*(entity->m_components.at(typeid(T)))).value;
    }

    template<typename T, typename Func>
    void each(const Func& func);

    std::vector<Entity*>::iterator begin() { return m_entities.begin(); }
    std::vector<Entity*>::iterator end()   { return m_entities.end(); }
    std::vector<Entity*>::const_iterator begin() const { return m_entities.begin(); }
    std::vector<Entity*>::const_iterator end()   const { return m_entities.end(); }

private:
    std::vector<Entity*> m_entities;
};

class Registry
{
public:
    Registry()
        : m_parent(nullptr)
    {
        
    }

    Registry(Entity* parent)
        : m_parent(parent)
    {

    }

    ~Registry()
    {
        
    }

    Entity* create()
    {
        m_entities.push_back(Entity(this));
        return &(m_entities.back());
    }

    void destroy(Entity* entity)
    {
        entity->m_children->clear();

        for (auto& component : entity->m_components)
        {
            delete component.second;
        }

        for (unsigned int i = 0; i < m_entities.size(); i++)
        {
            if (&m_entities[i] == entity)
            {
                m_entities.erase(m_entities.begin() + i);
            }
        }

        entity = nullptr;
    }

    template<typename T>
    T& emplace(Entity* entity)
    {
        if (has<T>(entity))
        {
            Logger::getCoreLogger()->warn("Entity already has specified component. (Registry::emplace)");
        }
        else
        {
            entity->m_components.insert(std::pair<std::type_index, IComponent*>(typeid(T), new Component<T>()));
        }

        return static_cast<Component<T>&>(*(entity->m_components.at(typeid(T)))).value;
    }

    template<typename T, typename... Args>
    T& emplace(Entity* entity, Args&&... args)
    {
        if (entity == nullptr)
        {
            Logger::getCoreLogger()->error("Entity is null! (Registry::emplace)");
        }
        if (has<T>(entity))
        {
            Logger::getCoreLogger()->warn("Entity already has specified component: %s (Registry::emplace)", typeid(T).name());
        }
        else
        {
            entity->m_components.insert(std::pair<std::type_index, IComponent*>(typeid(T), new Component<T>(std::forward<Args>(args)...)));
        }

        return static_cast<Component<T>&>(*(entity->m_components.at(typeid(T)))).value;
    }

    template<typename T>
    void insert()
    {
        
    }

    template<typename... Components>
    void remove(Entity* entity)
    {
        (internal_remove<Components>(entity), ...);
    }

    template<typename... Components>
    void remove_if_exists(Entity* entity)
    {
        (internal_remove_if_exists<Components>(m_entities), ...);
    }

    template<typename... Components>
    bool has(Entity* entity)
    {
        bool check = true;
        (internal_has<Components>(entity, check), ...);
        return check;
    }

    template<typename... Components>
    bool any(Entity* entity)
    {
        bool check = false;
        (internal_any<Components>(entity, check), ...);
        return check;
    }

    template<typename T>
    T& get(Entity* entity)
    {
        if (!has<T>(entity))
        {
            Logger::getCoreLogger()->error("Entity does not have specified component: %s (Registry::get)", typeid(T).name());
        }

        return static_cast<Component<T>&>(*(entity->m_components.at(typeid(T)))).value;
    }

    template<typename T>
    const T& get(Entity* entity) const
    {
        return get<T>(entity);
    }

    template<typename T, typename... Args>
    void replace(Entity* entity, Args&&... args)
    {
        delete entity->m_components.find(typeid(T))->second;
        entity->m_components.find(typeid(T))->second = new Component<T>(std::forward<Args>(args)...);
    }

    template<typename T, typename... Args>
    void emplace_or_replace(Entity* entity, Args&&... args)
    {
        if (has<T>(entity))
        {
            this->replace<T>(entity, std::forward<Args>(args)...);
        }
        else
        {
            this->emplace<T>(entity, std::forward<Args>(args)...);
        }
    }

    template<typename T, typename F>
    void patch(Entity* entity, F func)
    {
        if (!has<T>(entity))
        {
            Logger::getCoreLogger()->error("Cannot patch a component that does not exist!");
            return;
        }

        func(static_cast<Component<T>&>(*(entity->m_components.at(typeid(T)))).value);
    }

    void each(const std::function<void(Entity* entity)>& fn)
    {
        for (auto& entity : m_entities)
        {
            fn(&entity);
        }
    }

    void recurse_each(const std::function<void(Entity* entity)>& fn)
    {
        for (auto& entity : m_entities)
        {
            entity.getChildren()->each(fn);
            fn(&entity);
        }
    }

    template<typename... Components>
    void clear()
    {
        for (auto& entity : m_entities)
        {
            this->remove<Components...>(&entity);
        }
    }

    void clear()
    {
        for (auto& entity : m_entities)
        {
            this->destroy(&entity);
        }

        m_entities.clear();
    }

    template<typename... Components>
    View view()
    {
        std::vector<Entity*> entities;
        for (auto& entity : m_entities)
        {
            if (has<Components...>(&entity))
            {
                entities.push_back(&entity);
            }
        }

        return View(entities);
    }

    template<typename... Components>
    View recurse_view()
    {
        std::vector<Entity*> entities;
        for (auto& entity : m_entities)
        {
            internal_recurse_view(&entity, entities);
            if (has<Components...>(&entity))
            {
                entities.push_back(&entity);
            }
        }

        return View(entities);
    }

    Entity* owned_by()
    {
        return m_parent;
    }

private:
    std::vector<Entity> m_entities;
    Entity* m_parent = nullptr;

    template<typename T>
    void internal_remove(Entity* entity)
    {   
        entity->m_components.erase(typeid(T));
    }

    template<typename T>
    void internal_remove_if_exists(Entity* entity)
    {
        if (this->has<T>(entity))
        {
            internal_remove<T>(entity);
        }
    }

    template<typename T>
    void internal_has(Entity* entity, bool& check)
    {
        if (!entity) { return; }
        if (!check) return; // All components must be present
        check = entity->m_components.find(typeid(T)) != entity->m_components.end();
    }

    template<typename T>
    void internal_any(Entity* entity, bool& check)
    {
        if (!entity) { return; }
        if (check) return; // Only one component required
        check = entity->m_components.find(typeid(T)) != entity->m_components.end();
    }

    template<typename T>
    T& internal_get(Entity* entity)
    {
        return static_cast<Component<T>&>(*(entity->m_components.at(typeid(T)))).value;
    }

    template<typename... Components>
    void internal_recurse_view(Entity* parent, std::vector<Entity*>& entities)
    {
        for (auto& entity : parent->getChildren()->m_entities)
        {
            internal_recurse_view(&entity, entities);
            if (has<Components...>(&entity))
            {
                entities.push_back(&entity);
            }
        }
    }
};

template<typename T, typename Func>
void View::each(const Func& func)
{
    for (auto& entity : m_entities)
    {
        if (entity->getParent()->has<T>(entity))
        {
            func(entity, entity->getParent()->get<T>(entity));
        }
    }
}

}

}