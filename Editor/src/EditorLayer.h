#pragma once

#include <core/Layer.h>
#include <scene/Scene.h>
#include <scene/ecs/Registry.h>
#include <renderer/Framebuffer.h>
#include <maths/vector/vec2.h>
#include <util/OrthographicCameraController.h>
#include <renderer/text/TrueTypeFont.h>

#include <imgui/imgui.h>
#include <imguizmo/ImGuizmo.h>

#include "SceneHierarchy.h"
#include "MaterialsPanel.h"

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
    Shared<Scene> m_scene;
    Shared<Framebuffer> m_framebuffer;

    math::vec2 m_viewportSize;

    bool m_viewportFocused;
    bool m_viewportHovered;

    EditorCamera m_editorCamera;

    SceneHierarchy m_sceneHeirarchy;
    MaterialsPanel m_materialsPanel;

    bool m_playingScene = false;

    Shared<Texture2D> m_scenePlayButton;
    Shared<Texture2D> m_sceneStopButton;

    uint32_t m_gizmoType = 0;
};