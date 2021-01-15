#pragma once

#include <scene/Scene.h>

namespace Engine
{

class EnvironmentPanel
{
public:
    EnvironmentPanel();

    void setContext(Scene* scene) { m_context = scene; }

    void onImGuiRender();

private:
    Scene* m_context = nullptr;
};

}