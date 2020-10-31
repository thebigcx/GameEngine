#include <Engine.h>

#include "SandboxApp.h"

Sandbox::Sandbox()
{
    Shared<Texture2D> texture = Texture2D::create("Sandbox/assets/terrain.png");
    Assets::add<Texture2D>("texture", *texture);

    m_soundSource = SoundSource::loadFile("Sandbox/assets/monkeys.mp3");
    SoundEngine::play(*m_soundSource, true);

    m_batch = SpriteBatch::create();

    m_font = TrueTypeFont::create("Sandbox/assets/minecraftia.ttf", 48);

    for (int i = 0; i < 999; i++)
    {
        Sprite sprite((i % 10) * 100, i * 10, 100, 100);

        sprite.setColor(Color(1, 1, 1, 1));
        sprite.setTextureRect(FloatRect(32.f, 32.f, 16.f, 16.f));

        m_sprites.push_back(sprite);
    }

    Renderer2D::setClearColor(Color(0, 0, 0, 1));

    m_animation = Animation::create(Assets::get<Texture2D>("texture"));
    m_animation->setFrames({ 
        FloatRect(32, 32, 16, 16), 
        FloatRect(48, 32, 16, 16) 
    });
    m_animation->setFrameInterval(100.f);
}

Application* createApplication()
{
    return new SandboxApp();
}

void Sandbox::update()
{
    auto dt = Time::getDelta();
    const float speed = 0.5;

    if (Input::isKeyPressed(Key::A))
        m_camera.translate(Vector2f(-speed * dt, 0));

    if (Input::isKeyPressed(Key::D))
        m_camera.translate(Vector2f(speed * dt, 0));

    if (Input::isKeyPressed(Key::W))
        m_camera.translate(Vector2f(0, speed * dt));

    if (Input::isKeyPressed(Key::S))
        m_camera.translate(Vector2f(0, -speed * dt));

    if (Input::isKeyPressed(Key::Escape))
    {
        Application::get().quit();
    }

    Renderer2D::startFrame();

    m_sprites[0].setTextureRect(m_animation->getCurrentFrame());
    m_animation->update();

    m_batch->setTransformMatrix(m_camera.getViewMatrix());
    m_batch->start();

    for (auto& sprite : m_sprites)
    {
        m_batch->renderSprite(Assets::get<Texture2D>("texture"), sprite);
    }

    m_batch->flush();

    Renderer2D::renderText("Hello, world!", *m_font, Vector2f(500, 500), Vector2f(80, 80), Color(1, 0, 0, 1));

    Renderer2D::endFrame();
}

void Sandbox::handleEvent(const Event& event)
{
    
}