#include <renderer/PostProcessor.h>
#include <renderer/RenderCommand.h>
#include <renderer/Assets.h>
#include <renderer/Renderer.h>

namespace Engine
{

PostProcessor::PostProcessor()
{
    m_framebufferMesh = MeshFactory::quadMesh(-1, -1, 1, 1);

    Framebuffer::Specification spec;
    spec.width = 1280;
    spec.height = 720;
    spec.attachments = {
        { Framebuffer::Attachment::Color, Framebuffer::TextureSpecification(SizedTextureFormat::RGBA16F) },
        { Framebuffer::Attachment::Depth, Framebuffer::TextureSpecification(SizedTextureFormat::Depth) }
    };

    m_hdrBuffer = Framebuffer::create(spec);

    for (unsigned int i = 0; i < 2; i++)
    {
        Framebuffer::Specification pingPongSpec;
        pingPongSpec.width = 1280;
        pingPongSpec.height = 720;
        pingPongSpec.attachments = {
            { Framebuffer::Attachment::Color, Framebuffer::TextureSpecification(SizedTextureFormat::RGBA16F) }
        };

        m_pingPongBuffers[i] = Framebuffer::create(pingPongSpec);
    }
}

PostProcessor::~PostProcessor()
{

}

Reference<Framebuffer> PostProcessor::finishHdr(const Reference<Framebuffer>& framebuffer)
{
    m_framebufferMesh->vertexArray->bind();

    m_hdrBuffer->bind();

    RenderCommand::clear(RenderCommand::defaultClearBits());

    Assets::get<Shader>("EngineHDR_Pass")->bind();
    Assets::get<Shader>("EngineHDR_Pass")->setFloat("uExposure", Renderer::hdrExposure);

    glBindTextureUnit(0, framebuffer->getColorAttachment()); // TODO: platform independent

    RenderCommand::renderIndexed(m_framebufferMesh->vertexArray);

    m_hdrBuffer->unbind();
    
    return m_hdrBuffer;
}

Reference<Framebuffer> PostProcessor::finishHdrAndBloom(const Reference<Framebuffer>& framebuffer)
{
    m_framebufferMesh->vertexArray->bind();

    bool horizontal = true, firstIteration = true;
    int amount = 10;
    Assets::get<Shader>("EngineHDR_GaussianBlur")->bind();
    for (unsigned int i = 0; i < amount; i++)
    {
        m_pingPongBuffers[horizontal]->bind();

        Assets::get<Shader>("EngineHDR_GaussianBlur")->setInt("uHorizontal", horizontal);
        glBindTextureUnit(0, firstIteration ? framebuffer->getColorAttachment(1) : m_pingPongBuffers[!horizontal]->getColorAttachment());

        RenderCommand::renderIndexed(m_framebufferMesh->vertexArray);

        horizontal = !horizontal;
        if (firstIteration)
            firstIteration = false;
    }

    m_pingPongBuffers[1]->unbind();

    // HDR Pass
    m_hdrBuffer->bind();

    RenderCommand::clear(RenderCommand::defaultClearBits());

    Assets::get<Shader>("EngineHDR_Bloom_Pass")->bind();
    Assets::get<Shader>("EngineHDR_Bloom_Pass")->setFloat("uExposure", Renderer::hdrExposure);

    glBindTextureUnit(0, framebuffer->getColorAttachment()); // TODO: platform independent
    glBindTextureUnit(1, m_pingPongBuffers[!horizontal]->getColorAttachment());

    RenderCommand::renderIndexed(m_framebufferMesh->vertexArray);

    m_hdrBuffer->unbind();

    return m_hdrBuffer;
}

void PostProcessor::onWindowResize(uint32_t width, uint32_t height)
{
    m_hdrBuffer->resize(width, height);
}

}