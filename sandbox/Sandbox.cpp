#include <FGL.h>

#include "SandboxApp.h"

Sandbox::Sandbox()
{
    Texture2D texture;
    texture.loadFile("sandbox/assets/terrain.png");
    AssetManager::get().textures.add("texture", texture);

    m_soundBuffer.load("sandbox/assets/monkeys.mp3");
    m_soundSource.create();
    SoundEngine::playFromSource(m_soundBuffer, m_soundSource, true);

    font.load("sandbox/assets/minecraftia.ttf");

    for (int i = 0; i < 999; i++)
    {
        Quad quad;

        quad.setPosition(Vector2f((i % 10)*100, i*10));
        quad.setSize(Vector2f(100, 100));
        quad.setColor(Color(1, 1, 1, 1));
        quad.setTextureRect(FloatRect(32.f, 32.f, 16.f, 16.f));

        m_quads.push_back(quad);
    }

    Renderer::setClearColor(Color(0, 0, 0, 1));
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
        m_camera.translate(Vector2f(-speed * dt, 0));
    }
    if (Input::isKeyPressed(Key::D))
    {
        m_camera.translate(Vector2f(speed * dt, 0));
    }
    if (Input::isKeyPressed(Key::W))
    {
        m_camera.translate(Vector2f(0, speed * dt));
    }
    if (Input::isKeyPressed(Key::S))
    {
        m_camera.translate(Vector2f(0, -speed * dt));
    }

    if (Input::isKeyPressed(Key::Escape))
    {
        Application::get().quit();
    }

    Renderer::startFrame();

    m_batch.setTransformMatrix(m_camera.getViewMatrix());
    m_batch.start();

    for (auto& quad : m_quads)
    {
        m_batch.renderQuad(AssetManager::get().textures.get("texture"), quad);
    }

    m_batch.flush();

    Renderer::endFrame();
}

void Sandbox::handleEvent(const Event& event)
{
    
}