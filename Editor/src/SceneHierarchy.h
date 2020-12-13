#pragma once

#include <scene/Scene.h>
#include <core/Core.h>
#include <scene/SceneEntity.h>

class SceneHierarchy
{
public:
    SceneHierarchy() = default;
    SceneHierarchy(const Shared<Scene>& scene);

    template<typename T, typename F>
    void drawComponent(const std::string& name, SceneEntity& entity, const F& func);

    void drawProperties(SceneEntity& entity);
    
    void setContext(const Shared<Scene>& context)
    {
        m_context = context;
    }

    void onImGuiRender();

private:
    Shared<Scene> m_context;

    SceneEntity m_selection;
};