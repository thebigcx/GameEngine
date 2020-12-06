#pragma once

#include <Engine.h>

class Sandbox : public Layer
{
public:
    Sandbox();

    void onAttach() override;
    void onUpdate() override;
    void onEvent(Event& event) override;

private:
    Shared<Material> m_cubeMaterial;

    Shared<Model> m_model;

    Shared<TrueTypeFont> m_font;

    Shared<UniformBuffer> m_testUniformBuffer;

    LightSetup lights;

    PerspectiveCameraController m_perspectiveCamera;
    OrthographicCameraController m_orthoCamera;
};