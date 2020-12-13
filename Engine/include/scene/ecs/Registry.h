#pragma once

#include <vector>
#include <functional>
#include <typeindex>

#include <scene/ecs/Entity.h>

class EntityView
{
public:
    EntityView(const std::vector<Entity*>& entities)
        : m_entities(entities)
    {

    }

    template<typename T>
    T& get(Entity* pEntity)
    {
        Entity* entity = m_entities[std::find(m_entities.begin(), m_entities.end(), pEntity) - m_entities.begin()];
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

class EntityRegistry
{
public:
    EntityRegistry()
    {

    }

    ~EntityRegistry()
    {
        
    }

    Entity* create()
    {
        m_entities.push_back(Entity(this));
        return &(m_entities.back());
    }

    void destroy(Entity* entity)
    {
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
            Logger::getCoreLogger()->warn("Entity already has specified component.");
        }
        else
        {
            entity->m_components.insert(std::pair<std::type_index, IComponent*>(typeid(T), new Component<T>()));
        }

        return static_cast<Component<T>&>(*(entity->m_components.at(typeid(T)))).value;
    }

    template<typename T, typename... Args>
    T& emplace(Entity* entity, Args... args)
    {
        if (has<T>(entity))
        {
            Logger::getCoreLogger()->warn("Entity already has specified component.");
        }
        else
        {
            entity->m_components.insert(std::pair<std::type_index, IComponent*>(typeid(T), new Component<T>(args...)));
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
        return static_cast<Component<T>&>(*(entity->m_components.at(typeid(T)))).value;
    }

    template<typename T>
    const T& get(Entity* entity) const
    {
        return get<T>(entity);
    }

    template<typename T, typename... Args>
    void replace(Entity* entity, Args... args)
    {
        delete entity->m_components.find(typeid(T))->second;
        entity->m_components.find(typeid(T))->second = new Component<T>(args...);
    }

    template<typename T, typename... Args>
    void emplace_or_replace(Entity* entity, Args... args)
    {
        if (has<T>(entity))
        {
            this->replace<T>(entity, args...);
        }
        else
        {
            this->emplace<T>(entity, args...);
        }
    }

    template<typename T, typename F>
    void patch(Entity* entity, F func)
    {
        func(static_cast<Component<T>&>(*(entity->m_components.at(typeid(T)))).value);
    }

    /*template<typename... Components>
    std::tuple<Components...> get(Entity* entity)
    {
        std::tuple<Components...> components;
        (internal_get<Components>(entity), ...);
    }*/

    void each(const std::function<void(Entity* entity)>& fn)
    {
        for (auto& entity : m_entities)
        {
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

    /*template<typename T>
    EntityView view()
    {
        std::vector<Entity*> entities;
        for (auto& entity : m_entities)
        {
            if (has<T>(&entity))
            {
                entities.push_back(&entity);
            }
        }

        return EntityView(entities);
    }*/

    template<typename... Components>
    EntityView view()
    {
        std::vector<Entity*> entities;
        for (auto& entity : m_entities)
        {
            if (has<Components...>(&entity))
            {
                entities.push_back(&entity);
            }
        }

        return EntityView(entities);
    }

private:
    std::vector<Entity> m_entities;

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
        if (!check) return; // All components must be present
        check = entity->m_components.find(typeid(T)) != entity->m_components.end();
    }

    template<typename T>
    void internal_any(Entity* entity, bool& check)
    {
        if (check) return; // Only one component required
        check = entity->m_components.find(typeid(T)) != entity->m_components.end();
    }

    template<typename T>
    T& internal_get(Entity* entity)
    {
        return static_cast<Component<T>&>(*(entity->m_components.at(typeid(T)))).value;
    }
};