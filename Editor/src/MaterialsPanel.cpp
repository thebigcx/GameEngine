#include "MaterialsPanel.h"

#include <imgui/imgui.h>

MaterialsPanel::MaterialsPanel(const Shared<Scene>& context)
    : m_context(context)
{

}

void MaterialsPanel::onImGuiRender()
{
    ImGui::Begin("Materials");

    ImGui::End();
}