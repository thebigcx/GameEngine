#pragma once

#include <scene/Scene.h>

namespace Engine
{

class DebugPanel
{
public:
    DebugPanel();

    void setContext(Scene* scene) { m_context = scene; }
    
    void onImGuiRender();

private:
    Scene* m_context = nullptr;
};

}