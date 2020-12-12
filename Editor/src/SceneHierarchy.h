#pragma once

#include <scene/Scene.h>
#include <core/Core.h>

class SceneHierarchy
{
public:
    SceneHierarchy() = default;
    SceneHierarchy(const Shared<Scene>& scene);

    template<typename T, typename F>
    void drawComponent(const std::string& name, Entity* entity, const F& func);

    void drawProperties(Entity* entity);
    
    void setContext(const Shared<Scene>& context)
    {
        m_context = context;
    }

    void onImGuiRender();

private:
    Shared<Scene> m_context;

    Entity* m_selection;
};