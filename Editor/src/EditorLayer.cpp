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
    m_registry.emplace<TransformComponent>(entity, math::vec3(4, 6, 2), math::vec3(0), math::vec3(1));
    m_registry.emplace<TagComponent>(entity, "Entity 1");
    
    auto entity1 = m_registry.create();
    m_registry.emplace<TagComponent>(entity1, "Entity 2");
}

void EditorLayer::onUpdate()
{
    RenderCommand::clear(RenderCommand::defaultClearBits());
}

void EditorLayer::onImGuiRender()
{
    bool show = true;
    ImGui::ShowDemoWindow(&show);

    ImGui::Begin("Window 1");
    EntityView view = m_registry.view<TagComponent>();
    for (auto& entity : view)
    {
        auto& tag = view.get<TagComponent>(entity).tag;
        ImGui::Text(tag.c_str());
    }
    ImGui::End();

    ImGui::Begin("Window 2");
    m_registry.each([&](Entity* entity)
    {
        ImGui::Text(m_registry.get<TagComponent>(entity).tag.c_str());
    });
    
    ImGui::End();
}

void EditorLayer::onDetach()
{
    
}

void EditorLayer::onEvent(Event& event)
{
    
}