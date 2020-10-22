#include <FGL.h>

#include "SandboxApp.h"

Sandbox::Sandbox()
{
    m_texture.loadFile("sandbox/res/terrain.png");

    AssetManager::get().textures.add("texture", m_texture);

    m_soundBuffer.load(SoundBuffer::FileType::MP3, "sandbox/res/monkeys.mp3");
    m_soundSource.create();
    SoundEngine::playFromSource(m_soundBuffer, m_soundSource, true);

    m_batch.create(AssetManager::get().textures.get("texture"));

    m_quad.setPosition(Vector2f(100, 100));
    m_quad.setSize(Vector2f(100, 100));
    m_quad.setColor(Color(1, 1, 1, 1));
    m_quad.setTexture2DRect(FloatRect(15.f / 16.f, 15.f / 16.f , 1.f / 16.f, 1.f / 16.f));

    for (int i = 0; i < 9999; i++)
    {
        Quad quad;

        quad.setPosition(Vector2f(i, i));
        quad.setSize(Vector2f(100, 100));
        quad.setColor(Color(1, 1, 1, 1));
        quad.setTexture2DRect(FloatRect(15.f / 16.f, 15.f / 16.f , 1.f / 16.f, 1.f / 16.f));

        m_quads.push_back(quad);
    }
    

    Renderer::setClearColor(Color(1, 1, 1, 1));
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
    {
        m_quad.move(Vector2f(-speed * dt, 0));
    }
    if (Input::isKeyPressed(Key::D))
    {
        m_quad.move(Vector2f(speed * dt, 0));
    }
    if (Input::isKeyPressed(Key::W))
    {
        m_quad.move(Vector2f(0, speed * dt));
    }
    if (Input::isKeyPressed(Key::S))
    {
        m_quad.move(Vector2f(0, -speed * dt));
    }


    Renderer::startFrame();

    m_batch.start();

    for (auto& quad : m_quads)
    {
        m_batch.renderQuad(quad);
    }
    m_batch.renderQuad(m_quad);

    m_batch.flush();

    Renderer::endFrame();
}

void Sandbox::handleEvent(const Event& event)
{
    
}