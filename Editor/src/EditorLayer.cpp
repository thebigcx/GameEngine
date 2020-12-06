#include "EditorLayer.h"

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include <core/Application.h>
#include <renderer/RenderCommand.h>

EditorLayer::EditorLayer()
{

}

void EditorLayer::onAttach()
{
    
}

void EditorLayer::onUpdate()
{
    
}

void EditorLayer::onImGuiRender()
{
    bool show = true;
    ImGui::ShowDemoWindow(&show);

    ImGui::Begin("Hello, world!");
    ImGui::Text("This is a text.");
    ImGui::End();
}

void EditorLayer::onDetach()
{
    
}

void EditorLayer::onEvent(Event& event)
{
    
}