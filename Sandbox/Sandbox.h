#pragma once

#include <core/Layer.h>
#include <renderer/Texture2D.h>
#include <renderer/SpriteBatch.h>
#include <sound/SoundSource.h>
#include <sound/SoundBuffer.h>
#include <renderer/text/TrueTypeFont.h>
#include <renderer/Animation.h>

class Sandbox : public Layer
{
public:
    Sandbox();

    void update() override;
    void handleEvent(const Event& event) override;

private:
    Shared<SpriteBatch> m_batch;
    Shared<SoundSource> m_soundSource;
    Shared<TrueTypeFont> m_font;
    
    Shared<Animation> m_animation;
    float rot = 0;

    std::vector<Sprite> m_sprites;

    OrthographicCamera m_camera;
};