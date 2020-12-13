#include "EditorLayer.h"

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include <core/Application.h>
#include <renderer/RenderCommand.h>
#include <scene/Components.h>
#include <renderer/Renderer2D.h>
#include <renderer/Renderer.h>
#include <renderer/Assets.h>

EditorLayer::EditorLayer()
{

}

void EditorLayer::onAttach()
{
    Assets::add<Texture2D>("texture", Texture2D::create("Editor/assets/texture.png"));
    Assets::add<Texture2D>("texture_1", Texture2D::create("Editor/assets/texture_1.png"));
    Assets::add<Texture2D>("texture_2", Texture2D::create("Editor/assets/texture_2.png"));

    m_framebuffer = Framebuffer::create(1280, 720);
    m_viewportSize = math::vec2(1280, 720);

    m_scene = createShared<Scene>();
    m_scene->onViewportResize(m_viewportSize.x, m_viewportSize.y);

    m_sceneHeirarchy.setContext(m_scene);

    // TEMP
    auto entity1 = m_scene->createEntity("Square");
    entity1.addComponent<SpriteRendererComponent>();
    entity1.addComponent<TransformComponent>(math::vec3(100, 100, 0), math::vec3(0), math::vec3(200));

    auto entity2 = m_scene->createEntity("Camera");
    entity2.addComponent<TransformComponent>();
    entity2.addComponent<CameraComponent>();
}

void EditorLayer::onUpdate(float dt)
{
    m_editorCamera.onUpdate(dt);

    if (m_framebuffer->getWidth() != m_viewportSize.x || m_framebuffer->getHeight() != m_viewportSize.y)
    {
        m_editorCamera.setViewportSize(m_viewportSize.x, m_viewportSize.y);
        m_framebuffer->resize(m_viewportSize.x, m_viewportSize.y);
    }
    

    m_framebuffer->bind();
    RenderCommand::setClearColor(math::vec4(0, 0, 0, 1));
    RenderCommand::clear(RenderCommand::defaultClearBits());


    if (Input::isKeyPressed(Key::LeftAlt))
    {
        m_scene->onUpdateRuntime(dt);
    }
    else
    {
        m_scene->onUpdateEditor(dt, m_editorCamera);
    }
    
    //m_scene->onUpdateEditor(dt, m_editorCamera);

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

    ImGui::SetNextWindowSize(ImVec2{1280, 720});
    ImGui::Begin("Viewport");

    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	m_viewportSize = { viewportPanelSize.x, viewportPanelSize.y };

    ImGui::Image(reinterpret_cast<void*>(m_framebuffer->getColorAttachment()), ImVec2{m_viewportSize.x, m_viewportSize.y}, ImVec2{0, 1}, ImVec2{1, 0});
    ImGui::End();

    m_sceneHeirarchy.onImGuiRender();
    
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
    EventDispatcher dispatcher(event);
    dispatcher.dispatch<WindowResizeEvent>(BIND_EVENT_FN(EditorLayer::onViewportResize));
    m_editorCamera.onEvent(event);
}