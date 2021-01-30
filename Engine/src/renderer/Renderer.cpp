#include <renderer/Renderer.h>
#include <renderer/MeshFactory.h>
#include <renderer/shader/ShaderFactory.h>
#include <core/Game.h>
#include <events/WindowEvent.h>

namespace Engine
{

RendererData Renderer::m_data;
float Renderer::hdrExposure = 1.f;

void Renderer::init()
{
    RenderCommand::init();
    Renderer2D::init();
    Renderer3D::init();

    m_data.fboMesh = MeshFactory::quadMesh(-1, -1, 1, 1);
    m_data.fboShader = ShaderFactory::createShader("hdr");
    
    math::ivec2 windowSize = Game::getInstance()->getWindow().getSize();
    //m_data.target = Framebuffer::create(windowSize.x, windowSize.y);
    m_data.target = RenderTarget::create(windowSize.x, windowSize.y);
}

void Renderer::shutdown()
{
    Renderer2D::shutdown();
    Renderer3D::shutdown();
}

void Renderer::startFrame()
{
    m_data.target->bind();
    RenderCommand::clear(RenderCommand::defaultClearBits());
}

void Renderer::endFrame()
{
    m_data.target->unbind();

    RenderCommand::clear(RenderCommand::defaultClearBits());

    glBindTextureUnit(0, m_data.target->getColorAttachment());
    //m_data.target->getColorAttachment()->bind();

    m_data.fboShader->bind();
    m_data.fboMesh->vertexArray->bind();

    RenderCommand::renderIndexed(m_data.fboMesh->vertexArray);
}

void Renderer::windowResize(WindowResizeEvent& event)
{
    m_data.target->resize(event.getWidth(), event.getHeight());
}

}