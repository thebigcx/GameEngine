#include "DebugPanel.h"

#include <imgui/imgui.h>

namespace Engine
{

DebugPanel::DebugPanel()
{

}

void DebugPanel::onImGuiRender()
{
    ImGui::Begin("Debug");

    ImGui::End();
}

}