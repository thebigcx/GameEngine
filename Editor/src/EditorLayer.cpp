#include "EditorLayer.h"

#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include <core/Application.h>
#include <renderer/RenderCommand.h>
#include <scene/Components.h>
#include <renderer/Renderer2D.h>
#include <renderer/Renderer.h>
#include <renderer/Assets.h>
#include <renderer/MeshFactory.h>
#include <scene/SceneSerializer.h>
#include <script/lua/state.h>
#include <util/Timer.h>
#include <util/io/Files.h>

namespace Engine
{

EditorLayer::EditorLayer()
{

}

void EditorLayer::onAttach()
{
    Application::get().getWindow().setSize(math::uvec2(1920, 1080));

    m_framebuffer = Framebuffer::create(1280, 720);
    m_viewportSize = math::vec2(1280, 720);
    m_editorCamera = EditorCamera(30.f, 1280.f / 720.f, 0.1f, 100000.f);

    m_scene = createShared<Scene>();
    m_sceneHeirarchy.setContext(m_scene);
    m_materialsPanel.setContext(m_scene);
    m_scriptEngine.setContext(m_scene);

    m_scenePlayButton = Texture2D::create("Editor/assets/scene_play.png");
    m_sceneStopButton = Texture2D::create("Editor/assets/scene_stop.png");

    Assets::add<Shader>("shader", Shader::createFromFile("Engine/src/renderer/shader/default/pbr.glsl"));
    Assets::add<Material>("default", Material::create(Assets::get<Shader>("pbr")));

    {
        auto whiteTexture = Texture2D::create(1, 1);
        uint32_t white = 0xffffffff;
        whiteTexture->setData(0, 0, 1, 1, &white);
        Assets::add<Texture2D>("white_texture", whiteTexture);
    }

    auto script = m_scene->createGameObject("Script");
    auto& luaScript = script->addComponent<LuaScriptComponent>();
    luaScript.filePath = "Editor/scripts/test.lua";
}

void EditorLayer::onUpdate(float dt)
{
    Timer timer;
    m_editorCamera.onUpdate(dt);
    m_scriptEngine.onUpdate(dt);

    if (m_framebuffer->getWidth() != m_viewportSize.x || m_framebuffer->getHeight() != m_viewportSize.y)
    {
        m_editorCamera.setViewportSize(m_viewportSize.x, m_viewportSize.y);
        m_framebuffer->resize(m_viewportSize.x, m_viewportSize.y);
        m_scene->onViewportResize(m_viewportSize.x, m_viewportSize.y);
    }

    m_framebuffer->bind();
    RenderCommand::setClearColor(math::vec4(0, 0, 0, 1));
    RenderCommand::clear(RenderCommand::defaultClearBits());

    if (m_playingScene)
    {
        m_scene->onUpdateRuntime(dt);
    }
    else
    {
        m_scene->onUpdateEditor(dt, m_editorCamera);
    }

    m_framebuffer->unbind();

    Application::get().getWindow().setTitle(std::string("Frame time: ") + std::to_string(timer.getMillis()));
}

void EditorLayer::drawMenuBar()
{
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Open", "Ctrl+O"))
            {
                FileDialog::open("loadScene");
            }

            if (ImGui::MenuItem("Save", "Ctrl+S"))
            {
                //SceneSerializer::saveScene(m_scene, "");
            }

            if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
            {
                FileDialog::open("saveSceneAs");
                //SceneSerializer::saveScene(m_scene, "");
            }

            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    if (FileDialog::selectFile("loadScene", "Choose scene...", ".yaml"))
    {
        if (!FileDialog::display())
        {
            if (FileDialog::madeSelection())
            {
                Assets::flush();
                m_scene = SceneSerializer::loadScene(FileDialog::getSelection());
                m_sceneHeirarchy.setContext(m_scene);
                m_materialsPanel.setContext(m_scene); // TODO: add a callback system to make this better
            }
        }
    }

    if (FileDialog::saveFile("saveSceneAs", "Save scene...", ".yaml"))
    {
        if (!FileDialog::display())
        {
            if (FileDialog::madeSelection())
            {
                SceneSerializer::saveScene(m_scene, FileDialog::getSelection() + std::string("/") + FileDialog::getSaveFileName());
            }
        }
    }
    
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

    drawMenuBar();

    windowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse;
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{0, 0});
    ImGui::Begin("Scene Settings", &dockspaceOpen, windowFlags);

    if (ImGui::ImageButton(reinterpret_cast<void*>(m_scenePlayButton->getId()), ImVec2{30, 30}))
    {
        m_playingScene = true;
    }
    ImGui::SameLine();
    if (ImGui::ImageButton(reinterpret_cast<void*>(m_sceneStopButton->getId()), ImVec2{30, 30}))
    {
        m_playingScene = false;
    }

    ImGui::End();
    ImGui::PopStyleVar();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
    ImGui::Begin("Viewport");

    m_viewportFocused = ImGui::IsWindowFocused();
    m_viewportHovered = ImGui::IsWindowHovered();
    Application::get().getImGuiLayer()->blockEvents(!m_viewportFocused && !m_viewportHovered);

    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	m_viewportSize = { viewportPanelSize.x, viewportPanelSize.y };

    if (!m_playingScene)
    {
        ImGui::Image(reinterpret_cast<void*>(m_framebuffer->getColorAttachment()), ImVec2{m_viewportSize.x, m_viewportSize.y}, ImVec2{0, 1}, ImVec2{1, 0});
    }

    // ImGuizmo
    // TODO: create own gizmo library

    if (!m_playingScene)
    {
        const math::mat4& projection = m_editorCamera.getProjectionMatrix();
        const math::mat4& view = m_editorCamera.getViewMatrix();

        ImGuizmo::SetOrthographic(false);
        ImGuizmo::SetDrawlist();

        float windowWidth = (float)ImGui::GetWindowWidth();
        float windowHeight = (float)ImGui::GetWindowHeight();
        ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

        ImGuizmo::DrawGrid(math::buffer(view), math::buffer(projection), math::buffer(math::mat4(1.f)), 10);

        GameObject* object = m_sceneHeirarchy.getSelectedGameObject();
        if (object)
        {
            if (m_gizmoType != -1 && object->hasComponent<TransformComponent>())
            {
                auto& tc = object->getComponent<TransformComponent>();
                math::mat4 transform = tc.getTransform();

                bool snap = Input::isKeyPressed(Key::LeftControl);
                float snapValue = 0.5f;
                
                if (m_gizmoType == ImGuizmo::OPERATION::ROTATE)
                {
                    snapValue = 45.f;
                }

                float snapValues[] = { snapValue, snapValue, snapValue };
                
                ImGuizmo::Manipulate(math::buffer(view), math::buffer(projection), (ImGuizmo::OPERATION)m_gizmoType, ImGuizmo::LOCAL, math::buffer(transform), nullptr, snap ? snapValues : nullptr);
            
                if (ImGuizmo::IsUsing())
                {
                    math::vec3 translation, rotation, scale;
                    math::decompose_transform(transform, translation, rotation, scale);

                    math::vec3 deltaRotation = math::degrees(rotation) - tc.rotation;
                    tc.translation = translation;
                    tc.rotation += deltaRotation;
                    tc.scale = scale;
                }
            }
        }

    }
    
    ImGui::End();
    ImGui::PopStyleVar();

    if (m_playingScene)
    {
        ImGuiWindowFlags flags = ImGuiWindowFlags_NoDocking;
        ImGui::Begin("Game", nullptr, flags);

        m_viewportSize.x = ImGui::GetContentRegionAvail().x;
        m_viewportSize.y = ImGui::GetContentRegionAvail().y;

        ImGui::Image(reinterpret_cast<void*>(m_framebuffer->getColorAttachment()), ImVec2{m_viewportSize.x, m_viewportSize.y}, ImVec2{0, 1}, ImVec2{1, 0});

        ImGui::End();
    }

    m_sceneHeirarchy.onImGuiRender();
    m_materialsPanel.onImGuiRender();
    
    ImGui::End();
    ImGui::PopStyleVar();
}

void EditorLayer::onDetach()
{
    
}

void EditorLayer::onEvent(Event& event)
{
    EventDispatcher dispatcher(event);
    dispatcher.dispatch<KeyPressedEvent>(BIND_EVENT_FN(EditorLayer::onKeyPressed));
    m_editorCamera.onEvent(event);
}

bool EditorLayer::onKeyPressed(KeyPressedEvent& event)
{
    switch ((Key)event.getKeyCode())
    {
        case Key::Q:
            m_gizmoType = -1;
            break;
        case Key::W:
            m_gizmoType = ImGuizmo::OPERATION::TRANSLATE;
            break;
        case Key::E:
            m_gizmoType = ImGuizmo::OPERATION::ROTATE;
            break;
        case Key::R:
            m_gizmoType = ImGuizmo::OPERATION::SCALE;
            break;
        default:
            break;
    }

    return false;
}

}