#include <FGL.h>

#include "SandboxApp.h"

Sandbox::Sandbox()
{
    Texture2D texture;
    texture.loadFile("sandbox/res/terrain.png");
    AssetManager::get().textures.add("texture", texture);

    m_soundBuffer.load("sandbox/res/monkeys.mp3");
    m_soundSource.create();
    SoundEngine::playFromSource(m_soundBuffer, m_soundSource, true);

    m_batch.create(AssetManager::get().textures.get("texture"));

    m_quad.setTexture(AssetManager::get().textures.get("texture"));
    m_quad.setPosition(Vector2f(100, 100));
    m_quad.setSize(Vector2f(100, 100));
    m_quad.setColor(Color(1, 1, 1, 1));
    m_quad.setTextureRect(FloatRect(12.f, 12.f, 1.f, 1.f));

    for (int i = 0; i < 999; i++)
    {
        Quad quad;

        quad.setTexture(AssetManager::get().textures.get("texture"));
        quad.setPosition(Vector2f((i % 10)*100, i*10));
        quad.setSize(Vector2f(100, 100));
        quad.setColor(Color(1, 1, 1, 1));
        quad.setTextureRect(FloatRect(32.f, 32.f, 16.f, 16.f));

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

    Renderer::startFrame();

    m_batch.setTransformMatrix(m_camera.getViewMatrix());
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