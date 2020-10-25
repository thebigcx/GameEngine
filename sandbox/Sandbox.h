#pragma once

#include <core/Layer.h>
#include <renderer/Texture2D.h>
#include <renderer/QuadBatch.h>
#include <sound/SoundSource.h>
#include <sound/SoundBuffer.h>

class Sandbox : public Layer
{
public:
    Sandbox();

    void update() override;
    void handleEvent(const Event& event) override;

private:
    QuadBatch m_batch;

    SoundBuffer m_soundBuffer;
    SoundSource m_soundSource;

    TrueTypeFont m_font;
    TextMesh m_mesh;

    std::vector<Quad> m_quads;
    std::vector<QuadBatch> m_batches;

    Camera m_camera;
};