#pragma once

#include <core/Layer.h>
#include <maths/vector/vec2.h>

#include <imgui/imgui.h>
#include <imguizmo/ImGuizmo.h>

#include "panels/SceneHierarchyPanel.h"
#include "panels/MaterialsPanel.h"
#include "panels/SceneRendererPanel.h"
#include "panels/DebugPanel.h"
#include "panels/EnvironmentPanel.h"

namespace Engine
{

class Framebuffer;
class Scene;
class KeyPressedEvent;

class EditorLayer : public Layer
{
public:
    EditorLayer();

    bool onKeyPressed(KeyPressedEvent& event);

    void onAttach() override;
    void onDetach() override;
    void onUpdate(float dt) override;
    void onImGuiRender() override;
    void onEvent(Event& event) override;

private:
    void drawMenuBar();

private:
    Reference<Scene> m_scene;
    Reference<Framebuffer> m_framebuffer;
    Reference<Framebuffer> m_finalBuffer;
    Reference<Mesh> m_framebufferMesh;

    math::vec2 m_viewportSize;

    bool m_viewportFocused;
    bool m_viewportHovered;

    EditorCamera m_editorCamera;

    SceneHierarchyPanel m_sceneHeirarchyPanel;
    MaterialsPanel m_materialsPanel;
    SceneRendererPanel m_sceneRendererPanel;
    DebugPanel m_debugPanel;
    EnvironmentPanel m_environmentPanel;

    bool m_playingScene = false;

    Reference<Texture2D> m_sceneToggleButton;

    uint32_t m_gizmoType = 0;
};

}