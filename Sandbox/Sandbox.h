#pragma once

#include <core/Layer.h>
#include <renderer/Texture2D.h>
#include <renderer/QuadBatch.h>
#include <renderer/text/Text.h>
#include <sound/SoundSource.h>
#include <sound/SoundBuffer.h>

class Sandbox : public Layer
{
public:
    Sandbox();

    void update() override;
    void handleEvent(const Event& event) override;

private:
    Shared<QuadBatch> m_batch;
    Shared<SoundSource> m_soundSource;
    Shared<TrueTypeFont> m_font;

    std::vector<Quad> m_quads;

    Camera m_camera;
};