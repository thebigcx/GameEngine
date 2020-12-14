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
    m_editorCamera = EditorCamera(30.f, 1280.f / 720.f, 0.1f, 1000.f);

    m_scene = createShared<Scene>();
    m_scene->onViewportResize(m_viewportSize.x, m_viewportSize.y);

    m_sceneHeirarchy.setContext(m_scene);

    // TEMP

    class CameraController : public ScriptableEntity
    {
    public:
        virtual void onCreate() override
        {

        }

        virtual void onDestroy() override
        {

        }

        virtual void onUpdate(float dt) override
        {
            auto& translation = getComponent<TransformComponent>().translation;

            float speed = 0.5f;

            if (Input::isKeyPressed(Key::A))
            {
                translation.x -= speed * dt;
            }
            if (Input::isKeyPressed(Key::D))
            {
                translation.x += speed * dt;
            }
            if (Input::isKeyPressed(Key::W))
            {
                translation.y += speed * dt;
            }
            if (Input::isKeyPressed(Key::S))
            {
                translation.y -= speed * dt;
            }
        }
    };

    auto entity1 = m_scene->createEntity("Square");
    auto& comp = entity1.addComponent<SpriteRendererComponent>();
    comp.texture = Texture2D::create("Editor/assets/texture.png");
    entity1.addComponent<TransformComponent>(math::vec3(0, 0, 0), math::vec3(0), math::vec3(200));

    entity1.addComponent<NativeScriptComponent>().bind<CameraController>();

    auto entity2 = m_scene->createEntity("Camera");
    entity2.addComponent<TransformComponent>();
    entity2.addComponent<CameraComponent>();

    // TEMP
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


    /*if (Input::isKeyPressed(Key::LeftAlt))
    {
        m_scene->onUpdateRuntime(dt);
    }
    else
    {
        m_scene->onUpdateEditor(dt, m_editorCamera);
    }*/
    
    m_scene->onUpdateEditor(dt, m_editorCamera);

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