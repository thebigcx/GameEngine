#pragma once

#include <core/Layer.h>
#include <scene/Scene.h>
#include <scene/ecs/Registry.h>
#include <renderer/Framebuffer.h>
#include <maths/vector/vec2.h>
#include <util/OrthographicCameraController.h>
#include "SceneHeirarchy.h"

class EditorLayer : public Layer
{
public:
    EditorLayer();

    void onViewportResize(WindowResizeEvent& event);

    void onAttach() override;
    void onDetach() override;
    void onUpdate() override;
    void onImGuiRender() override;
    void onEvent(Event& event) override;

private:
    Shared<Scene> m_scene;
    Shared<Framebuffer> m_framebuffer;

    math::vec2 m_viewportSize;

    OrthographicCameraController m_camera;

    SceneHeirarchy m_sceneHeirarchy;
};