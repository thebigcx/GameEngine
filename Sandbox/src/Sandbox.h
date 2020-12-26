#pragma once

#include <Engine.h>

class Sandbox : public Engine::Layer
{
public:
    Sandbox();

    void onAttach() override;
    void onDetach() override;
    void onUpdate(float dt) override;
    void onEvent(Engine::Event& event) override;

private:
    Engine::Shared<Engine::Material> m_cubeMaterial;

    Engine::Shared<Engine::Model> m_model;

    Engine::Shared<Engine::TrueTypeFont> m_font;

    Engine::Shared<Engine::UniformBuffer> m_testUniformBuffer;

    Engine::LightSetup lights;

    Engine::PerspectiveCameraController m_perspectiveCamera;
    Engine::Shared<Engine::Mesh> m_mesh;
};