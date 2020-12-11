#include "EditorLayer.h"

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include <core/Application.h>
#include <renderer/RenderCommand.h>
#include <scene/Components.h>
#include <renderer/Renderer2D.h>
#include <renderer/Renderer.h>

EditorLayer::EditorLayer()
{

}

void EditorLayer::onAttach()
{
    m_scene = createShared<Scene>();

    m_sceneHeirarchy.setContext(m_scene);
    m_framebuffer = Framebuffer::create(1280, 720);
    m_viewportSize = math::vec2(1280, 720);
    m_camera.setPosition(math::vec2(0, 0));
}

void EditorLayer::onUpdate()
{
    m_framebuffer->resize(m_viewportSize.x, m_viewportSize.y);

    m_framebuffer->bind();
    RenderCommand::setClearColor(math::vec4(0, 0, 0, 1));
    RenderCommand::clear(RenderCommand::defaultClearBits());

    Renderer2D::beginScene(m_camera);
    Renderer2D::renderQuad(math::vec2(100, 100), math::vec2(100, 100), math::vec4(1, 0, 0, 1));
    Renderer2D::endScene();

    m_scene->onUpdate();

    m_framebuffer->unbind();
}

void EditorLayer::onImGuiRender()
{
    ImGuiViewport* viewport = ImGui::GetMainViewport();

    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

    bool dockspaceOpen = true;

    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    ImGui::Begin("Window", &dockspaceOpen, windowFlags);
    ImGui::DockSpace(ImGui::GetID("MyDockSpace"), ImVec2(0.f, 0.f), ImGuiDockNodeFlags_None);

    ImGui::Begin("Viewport");

    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	m_viewportSize = { viewportPanelSize.x, viewportPanelSize.y };

    ImGui::Image(reinterpret_cast<void*>(m_framebuffer->getColorAttachment()), ImVec2{m_viewportSize.x, m_viewportSize.y}, ImVec2{0, 1}, ImVec2{1, 0});
    ImGui::End();
    
    ImGui::End();
    ImGui::PopStyleVar();
}

void EditorLayer::onDetach()
{
    
}

void EditorLayer::onViewportResize(WindowResizeEvent& event)
{
    m_scene->onViewportResize(event.getWidth(), event.getHeight());
}

void EditorLayer::onEvent(Event& event)
{
    m_camera.onEvent(event);
}