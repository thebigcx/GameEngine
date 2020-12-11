#pragma once

#include <scene/Scene.h>
#include <core/Core.h>

class SceneHeirarchy
{
public:
    SceneHeirarchy() = default;
    SceneHeirarchy(const Shared<Scene>& scene);
    
    void setContext(const Shared<Scene>& context)
    {
        m_context = context;
    }

    void onImGuiRender();

private:
    Shared<Scene> m_context;
};