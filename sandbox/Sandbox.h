#pragma once

#include <core/Layer.h>
#include <renderer/Texture2D.h>
#include <renderer/QuadBatch.h>
#include <renderer/ui/text/Text.h>
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

    TextMesh m_mesh;
    TrueTypeFont m_font;
    Text m_text;

    std::vector<Quad> m_quads;

    Camera m_camera;
};