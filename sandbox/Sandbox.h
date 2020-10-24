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

    std::vector<Quad> m_quads;
    std::vector<QuadBatch> m_batches;

    TextMesh m_mesh;
    Font m_font;
    UITextComponent m_text;

    Camera m_camera;
};