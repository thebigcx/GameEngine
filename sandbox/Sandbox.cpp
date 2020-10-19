#include <FGL.h>

#include "SandboxApp.h"

Sandbox::Sandbox()
{
    Shader textureShader;
    textureShader.create("shaders/texture.vert", "shaders/texture.frag");

    ShaderLibrary::add("texture", textureShader);
    ShaderLibrary::get("texture").bind();

    m_texture.loadFile("sandbox/res/terrain.png");
    m_texture.bind();

    glm::mat4 projection = glm::ortho(0.f, 1280.f, 0.f, 720.f, -1.f, 1.f);
    ShaderLibrary::get("texture").setUniform("projection", projection);

    m_batch.create(ShaderLibrary::get("texture"));
    m_batch.add(m_quad);
    m_quad = Quad(Vector2f(100, 100), Vector2f(100, 100), Color(1, 1, 1, 1));
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