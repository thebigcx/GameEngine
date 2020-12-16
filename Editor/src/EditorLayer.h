#pragma once

#include <core/Layer.h>
#include <scene/Scene.h>
#include <scene/ecs/Registry.h>
#include <renderer/Framebuffer.h>
#include <maths/vector/vec2.h>
#include <util/OrthographicCameraController.h>
#include <renderer/text/TrueTypeFont.h>

#include "SceneHierarchy.h"
#include "MaterialsPanel.h"

class EditorLayer : public Layer
{
public:
    EditorLayer();

    void onViewportResize(WindowResizeEvent& event);

    void onAttach() override;
    void onDetach() override;
    void onUpdate(float dt) override;
    void onImGuiRender() override;
    void onEvent(Event& event) override;

private:
    Shared<Scene> m_scene;
    Shared<Framebuffer> m_framebuffer;

    math::vec2 m_viewportSize;

    EditorCamera m_editorCamera;

    SceneHierarchy m_sceneHeirarchy;
    MaterialsPanel m_materialsPanel;

    bool m_playingScene = false;
};