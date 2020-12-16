#pragma once

#include <core/Core.h>
#include <scene/Scene.h>

class MaterialsPanel
{
public:
    MaterialsPanel() = default;
    MaterialsPanel(const Shared<Scene>& context);

    void setContext(const Shared<Scene>& context)
    {
        m_context = context;
    }

    void onImGuiRender();

private:
    Shared<Scene> m_context;
};