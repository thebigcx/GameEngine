#pragma once

#include <Engine.h>

class Sandbox : public Layer
{
public:
    Sandbox();

    void update() override;
    void handleEvent(const Event& event) override;

private:
    Shared<SoundSource> m_soundSource;
    Shared<Material> m_cubeMaterial;

    Shared<Model> m_model;

    PerspectiveCamera m_perspectiveCamera;
};