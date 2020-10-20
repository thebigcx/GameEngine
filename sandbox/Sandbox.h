#pragma once

#include <core/Layer.h>
#include <renderer/Texture.h>
#include <renderer/QuadBatch.h>

class Sandbox : public Layer
{
public:
    Sandbox();

    void update() override;
    void handleEvent(const Event& event) override;

private:
    Texture m_texture;

    QuadBatch m_batch;
    Quad m_quad;

    SoundBuffer m_soundBuffer;
    SoundSource m_soundSource;

    std::vector<Quad> m_quads;
};