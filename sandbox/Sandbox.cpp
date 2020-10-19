#include <FGL.h>

#include "SandboxApp.h"

Sandbox::Sandbox()
{
    Shader textureShader;
    textureShader.create("shaders/texture.vert", "shaders/texture.frag");

    ShaderLibrary::add("texture", textureShader);
    ShaderLibrary::get("texture").bind();

    m_texture.loadFile("sandbox/res/terrain.png");

    glm::mat4 projection = glm::ortho(0.f, 1280.f, 0.f, 720.f, -1.f, 1.f);
    ShaderLibrary::get("texture").setUniform("projection", projection);

    m_soundBuffer.load(SoundBuffer::FileType::MP3, "sandbox/res/beat.mp3");
    m_soundSource.create();
    SoundEngine::playFromSource(m_soundBuffer, m_soundSource, true);

    m_states = RenderStates::createStates(m_texture, ShaderLibrary::get("texture"), glm::mat4(1.f));
    m_batch.create(m_states);
    m_batch.add(m_quad);
    m_quad = Quad(Vector2f(100, 100), Vector2f(100, 100), Color(1, 1, 1, 1));
    m_quad.setTextureRect(FloatRect(15/16, 15/16, 1/16, 1/16));
}

Application* createApplication()
{
    return new SandboxApp();
}

void Sandbox::update()
{
    if (Keyboard::isKeyPressed(Keyboard::Key::A))
    {
        m_quad.move(Vector2f(-10, 0));
    }
    if (Keyboard::isKeyPressed(Keyboard::Key::D))
    {
        m_quad.move(Vector2f(10, 0));
    }
    if (Keyboard::isKeyPressed(Keyboard::Key::W))
    {
        m_quad.move(Vector2f(0, 10));
    }
    if (Keyboard::isKeyPressed(Keyboard::Key::S))
    {
        m_quad.move(Vector2f(0, -10));
    }

    Renderer::startRender();
    
    m_batch.update();
    Renderer::renderBatch(m_batch);

    Renderer::finishRender();
}

void Sandbox::handleEvent(const Event& event)
{
    
}