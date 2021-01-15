#include "SceneRendererPanel.h"

#include <imgui/imgui.h>

namespace Engine
{

SceneRendererPanel::SceneRendererPanel()
{

}

void SceneRendererPanel::onImGuiRender()
{
    ImGui::Begin("Scene Renderer");

    if (ImGui::CollapsingHeader("Shadows"))
    {

    }

    if (ImGui::CollapsingHeader("Bloom"))
    {

    }

    ImGui::End();
}

}