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
    auto entity = m_registry.create();
    entity->addComponent<TransformComponent>();
    entity->getComponent<TransformComponent>().rotation = math::vec3(10);
}

void EditorLayer::onUpdate()
{
    RenderCommand::clear(RenderCommand::defaultClearBits());
}

void EditorLayer::onImGuiRender()
{
    bool show = true;
    ImGui::ShowDemoWindow(&show);

    ImGui::Begin("Hello, world!");
    m_registry.each([=](Entity* entity)
    {
        ImGui::Text(std::to_string(entity->getComponent<TransformComponent>().rotation.x).c_str());
    });
    
    ImGui::End();
}

void EditorLayer::onDetach()
{
    
}

void EditorLayer::onEvent(Event& event)
{
    
}