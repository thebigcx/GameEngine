#include <FGL.h>

#include "SandboxApp.h"

Sandbox::Sandbox()
{
    Shader textureShader;
    textureShader.create("shaders/texture.vert", "shaders/texture.frag");

    ShaderLibrary::add("texture", textureShader);
    ShaderLibrary::get("texture").bind();

    m_texture.loadFile("sandbox/res/terrain.png");

    AssetManager::get().textures.add("texture", m_texture);

    glm::mat4 projection = glm::ortho(0.f, 1280.f, 0.f, 720.f, -1.f, 1.f);
    ShaderLibrary::get("texture").setUniform("projection", projection);

    m_soundBuffer.load(SoundBuffer::FileType::MP3, "sandbox/res/beat.mp3");
    m_soundSource.create();
    SoundEngine::playFromSource(m_soundBuffer, m_soundSource, true);

    RenderStates states = RenderStates::createStates(AssetManager::get().textures.get("texture"), 
                                                     ShaderLibrary::get("texture"),
                                                     glm::mat4(1.f),
                                                     BlendMode::Alpha);

    m_batch.create(states);

    m_quad.setPosition(Vector2f(100, 100));
    m_quad.setSize(Vector2f(100, 100));
    m_quad.setColor(Color(1, 1, 1, 1));
    m_quad.setTextureRect(FloatRect(15.f / 16.f, 15.f / 16.f , 1.f / 16.f, 1.f / 16.f));

    for (int i = 0; i < 9999; i++)
    {
        Quad quad;

        quad.setPosition(Vector2f(100, 100));
        quad.setSize(Vector2f(100, 100));
        quad.setColor(Color(1, 1, 1, 1));
        quad.setTextureRect(FloatRect(15.f / 16.f, 15.f / 16.f , 1.f / 16.f, 1.f / 16.f));

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
    const float speed = 5;
    if (Keyboard::isKeyPressed(Keyboard::Key::A))
    {
        m_quad.move(Vector2f(-speed, 0));
    }
    if (Keyboard::isKeyPressed(Keyboard::Key::D))
    {
        m_quad.move(Vector2f(speed, 0));
    }
    if (Keyboard::isKeyPressed(Keyboard::Key::W))
    {
        m_quad.move(Vector2f(0, speed));
    }
    if (Keyboard::isKeyPressed(Keyboard::Key::S))
    {
        m_quad.move(Vector2f(0, -speed));
    }
    Renderer::startRender();
    
    m_batch.begin();
    for (auto& quad : m_quads)
    {
        m_batch.draw(quad);
    }
    m_batch.draw(m_quad);
    m_batch.flush();

    Renderer::finishRender();
}

void Sandbox::handleEvent(const Event& event)
{
    
}