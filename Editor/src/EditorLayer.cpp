#include "EditorLayer.h"

#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include <core/Game.h>
#include <renderer/RenderCommand.h>
#include <scene/Components.h>
#include <renderer/Renderer2D.h>
#include <renderer/Assets.h>
#include <renderer/MeshFactory.h>
#include <scene/SceneSerializer.h>
#include <util/Timer.h>
#include <util/io/FileSystem.h>
#include <desktop/ImGuiLayer.h>
#include <events/EventDispatcher.h>
#include <core/Keyboard.h>
#include <events/KeyboardEvent.h>
#include <util/io/Serializer.h>
#include <renderer/Renderer.h>
#include <renderer/PostProcessor.h>

namespace Engine
{

EditorLayer::EditorLayer()
{

}

void EditorLayer::onAttach()
{
    Game::getInstance()->getWindow().setSize(math::uvec2(1920, 1080));
    FileSystem::setAssetDirectoryPath("Editor/assets/");

    Framebuffer::Specification spec;
    spec.width = 1280;
    spec.height = 720;
    spec.attachments = {
        { Framebuffer::Attachment::Color, Framebuffer::TextureSpecification(SizedTextureFormat::RGBA16F) },
        { Framebuffer::Attachment::Color, Framebuffer::TextureSpecification(SizedTextureFormat::RGBA16F) },
        { Framebuffer::Attachment::Depth, Framebuffer::TextureSpecification(SizedTextureFormat::Depth) }
    };

    m_framebuffer = Framebuffer::create(spec);
    m_viewportSize = math::vec2(1280, 720);
    m_editorCamera = EditorCamera(30.f, 1280.f / 720.f, 0.1f, 100000.f);

    m_scene = Scene::create();
    
    m_sceneHeirarchyPanel.setContext(m_scene.get());
    m_materialsPanel.setContext(m_scene.get());
    m_sceneRendererPanel.setContext(m_scene.get());
    m_debugPanel.setContext(m_scene.get());
    m_environmentPanel.setContext(m_scene.get());

    m_sceneToggleButton = Texture2D::create(FileSystem::getAssetPath("scene_toggle.png"));
}

void EditorLayer::onUpdate(float dt)
{
    Timer timer;
    m_editorCamera.onUpdate(dt);

    if (m_framebuffer->getWidth() != m_viewportSize.x || m_framebuffer->getHeight() != m_viewportSize.y)
    {
        m_editorCamera.setViewportSize(m_viewportSize.x, m_viewportSize.y);
        m_framebuffer->resize(m_viewportSize.x, m_viewportSize.y);
        m_scene->onViewportResize(m_viewportSize.x, m_viewportSize.y);

        PostProcessor::getInstance()->onWindowResize(m_viewportSize.x, m_viewportSize.y);
    }

    RenderCommand::setViewport(0, 0, m_viewportSize.x, m_viewportSize.y);
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

    m_finalBuffer = PostProcessor::getInstance()->finishHdrAndBloom(m_framebuffer);
    //m_finalBuffer = PostProcessor::getInstance()->finishHdr(m_framebuffer);

    Game::getInstance()->getWindow().setTitle(std::string("Frame time: ") + std::to_string(timer.getMillis()));
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
                if (m_scene->getPath() != "")
                {
                    Serializer::saveScene(m_scene, m_scene->getPath());
                }
            }

            if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
            {
                FileDialog::open("saveSceneAs");
            }

            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    if (FileDialog::selectFile("loadScene", "Choose scene...", ".scene"))
    {
        if (!FileDialog::display())
        {
            if (FileDialog::madeSelection())
            {
                m_scene.reset();
                m_scene = Deserializer::loadScene(FileDialog::getSelection());
                m_scene->onViewportResize(static_cast<uint32_t>(m_viewportSize.x), static_cast<uint32_t>(m_viewportSize.y));

                m_sceneHeirarchyPanel.setContext(m_scene.get());
                m_materialsPanel.setContext(m_scene.get()); // TODO: This is dumb.
                m_sceneRendererPanel.setContext(m_scene.get());
                m_debugPanel.setContext(m_scene.get());
                m_environmentPanel.setContext(m_scene.get());
            }
        }
    }

    if (FileDialog::saveFile("saveSceneAs", "Save scene...", ".scene"))
    {
        if (!FileDialog::display())
        {
            if (FileDialog::madeSelection())
            {
                Serializer::saveScene(m_scene, FileDialog::getSelection() + "/" + FileDialog::getSaveFileName());
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

    if (ImGui::ImageButton(reinterpret_cast<void*>(m_sceneToggleButton->getId()), ImVec2{30, 30}))
    {
        if (m_playingScene)
            m_scene->onSceneFinish();
        else
            m_scene->onSceneStart();
            
        m_playingScene = !m_playingScene;
    }

    ImGui::End();
    ImGui::PopStyleVar();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
    ImGui::Begin("Viewport");

    m_viewportFocused = ImGui::IsWindowFocused();
    m_viewportHovered = ImGui::IsWindowHovered();
    Game::getInstance()->getImGuiLayer()->blockEvents(!m_viewportFocused && !m_viewportHovered);

    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	m_viewportSize = { viewportPanelSize.x, viewportPanelSize.y };

    if (!m_playingScene)
    {
        ImGui::Image(reinterpret_cast<void*>(m_finalBuffer->getColorAttachment()), ImVec2{m_viewportSize.x, m_viewportSize.y}, ImVec2{0, 1}, ImVec2{1, 0});
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

        //ImGuizmo::DrawGrid(math::buffer(view), math::buffer(projection), math::buffer(math::mat4(1.f)), 10);
        // TODO: create grid myself, so depth information is correct

        GameObject* object = m_sceneHeirarchyPanel.getSelectedGameObject();
        if (object)
        {
            if (m_gizmoType != -1 && object->hasComponent<Transform>())
            {
                auto tc = object->getComponent<Transform>();
                math::mat4 transform = tc->matrix();
                math::mat4 worldTransform = tc->worldMatrix();

                bool snap = Keyboard::isPressed(Keyboard::Key::LeftControl);
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

                    math::vec3 deltaRotation = math::degrees(rotation) - tc->getRotation();
                    tc->setTranslation(translation);
                    tc->rotate(deltaRotation);
                    tc->setScale(scale);
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

    m_sceneHeirarchyPanel.onImGuiRender();
    m_materialsPanel.onImGuiRender();
    m_sceneRendererPanel.onImGuiRender();
    m_debugPanel.onImGuiRender();
    m_environmentPanel.onImGuiRender();

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
    switch (static_cast<Keyboard::Key>(event.getKeyCode()))
    {
        case Keyboard::Key::Q:
            m_gizmoType = -1;
            break;
        case Keyboard::Key::W:
            m_gizmoType = ImGuizmo::OPERATION::TRANSLATE;
            break;
        case Keyboard::Key::E:
            m_gizmoType = ImGuizmo::OPERATION::ROTATE;
            break;
        case Keyboard::Key::R:
            m_gizmoType = ImGuizmo::OPERATION::SCALE;
            break;
        default:
            break;
    }

    return false;
}

}
