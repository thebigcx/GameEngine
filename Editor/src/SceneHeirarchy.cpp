#include "SceneHeirarchy.h"

#include <imgui/imgui.h>

SceneHeirarchy::SceneHeirarchy(const Shared<Scene>& scene)
    : m_context(scene)
{

}

void SceneHeirarchy::onImGuiRender()
{
    ImGui::Begin("Scene Heirarchy");

    ImGui::End();
}