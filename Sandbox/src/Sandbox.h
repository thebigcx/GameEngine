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

    Shared<Mesh> m_skyboxMesh;
    Shared<TextureCube> m_skyboxTexture;
    Shared<Shader> m_skyboxShader;

    Shared<UniformBuffer> m_uniformBuffer;
    Shared<UniformBuffer> m_testUniformBuffer;

    LightSetup lights;

    PerspectiveCamera m_perspectiveCamera;
};