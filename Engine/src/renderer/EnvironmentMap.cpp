#include <renderer/EnvironmentMap.h>
#include <renderer/Texture2D.h>
#include <util/ImageLoader.h>
#include <renderer/Framebuffer.h>
#include <renderer/MeshFactory.h>
#include <renderer/RenderCommand.h>


namespace Engine
{

EnvironmentMap::EnvironmentMap(const std::string& hdrFile)
{
    Shared<HDRImage> image = ImageLoader::loadHDRImage(hdrFile);

    Shared<Texture2D> hdrTexture = Texture2D::create(image->width, image->height, GL_RGB16F, true, true);
    hdrTexture->setData(0, 0, image->width, image->height, image->data, GL_RGB, GL_FLOAT);

    m_envMap = TextureCube::create(512, 512, GL_RGB16F, true, true);

    if (!s_convertShader)
        s_convertShader = Shader::createFromFile("Engine/src/renderer/shader/default/equirectangular_to_cubemap.glsl");

    if (!s_irradianceShader)
        s_irradianceShader = Shader::createFromFile("Engine/src/renderer/shader/default/irradiance.glsl");

    math::mat4 captureProjection = math::perspective((float)math::radians(90.f), 1.f, 0.1f, 10.f);
    math::mat4 captureViews[] =
    {
        math::lookAt(math::vec3(0.f), math::vec3( 1.f,  0.f,  0.f), math::vec3(0.f, -1.f,  0.f)),
        math::lookAt(math::vec3(0.f), math::vec3(-1.f,  0.f,  0.f), math::vec3(0.f, -1.f,  0.f)),
        math::lookAt(math::vec3(0.f), math::vec3( 0.f,  1.f,  0.f), math::vec3(0.f,  0.f,  1.f)),
        math::lookAt(math::vec3(0.f), math::vec3( 0.f, -1.f,  0.f), math::vec3(0.f,  0.f, -1.f)),
        math::lookAt(math::vec3(0.f), math::vec3( 0.f,  0.f,  1.f), math::vec3(0.f, -1.f,  0.f)),
        math::lookAt(math::vec3(0.f), math::vec3( 0.f,  0.f, -1.f), math::vec3(0.f, -1.f,  0.f))
    };

    if (!s_cubeMesh)
    {
        s_cubeMesh = MeshFactory::cubeMesh(2.f);
    }

    Shared<Framebuffer> framebuffer = Framebuffer::create();
    framebuffer->bind();
    RenderCommand::setViewport(0, 0, 512, 512);
    Shared<Renderbuffer> renderbuffer = Renderbuffer::create(512, 512, GL_DEPTH_COMPONENT24);
    framebuffer->attachRenderbuffer(*renderbuffer, Attachment::Depth);
    // TODO: platform independent and refactor

    s_convertShader->bind();
    s_convertShader->setMatrix4("projection", captureProjection);

    hdrTexture->bind(0);
    framebuffer->bind();

    for (uint32_t i = 0; i < 6; i++)
    {
        s_convertShader->setMatrix4("view", captureViews[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_envMap->getId(), 0); // TODO: IMPORTANT! make platform independent

        s_cubeMesh->vertexArray->bind();
        RenderCommand::clear(RenderCommand::defaultClearBits());
        RenderCommand::renderIndexed(s_cubeMesh->vertexArray);
    }

    framebuffer->unbind();

    m_envMap->bind();// TODO: platform independent    
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    m_irradianceMap = TextureCube::create(32, 32, GL_RGB16F, true, true);

    framebuffer->bind();
    renderbuffer = Renderbuffer::create(32, 32, GL_DEPTH_COMPONENT24);
    framebuffer->attachRenderbuffer(*renderbuffer, Attachment::Depth);

    s_irradianceShader->bind();
    s_irradianceShader->setMatrix4("projection", captureProjection);
    m_envMap->bind(0);

    framebuffer->bind();
    for (uint32_t i = 0; i < 6; i++) // ++i?
    {
        s_irradianceShader->setMatrix4("view", captureViews[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_irradianceMap->getId(), 0); // TODO: IMPORTANT! make platform independent

        s_cubeMesh->vertexArray->bind();
        RenderCommand::clear(RenderCommand::defaultClearBits());
        RenderCommand::renderIndexed(s_cubeMesh->vertexArray);
    }

    framebuffer->unbind();

    m_prefilterMap = TextureCube::create(128, 128, GL_RGB16F, true, true, true);

    if (!s_prefilterShader)
        s_prefilterShader = Shader::createFromFile("Engine/src/renderer/shader/default/prefilter.glsl");

    s_prefilterShader->bind();
    s_prefilterShader->setMatrix4("projection", captureProjection);

    m_envMap->bind(0);

    framebuffer->bind();

    unsigned int mipmapLevels = 5;
    for (unsigned int mip = 0; mip < mipmapLevels; mip++)
    {
        unsigned int mipWidth = 128 * std::pow(0.5, mip);
        unsigned int mipHeight = 128 * std::pow(0.5, mip);

        renderbuffer = Renderbuffer::create(mipWidth, mipHeight, GL_DEPTH_COMPONENT24);
        RenderCommand::setViewport(0, 0, mipWidth, mipHeight);

        float roughness = (float)mip / (float)(mipmapLevels - 1);
        s_prefilterShader->setFloat("roughness", roughness);

        for (unsigned int i = 0; i < 6; ++i)
        {
            s_prefilterShader->setMatrix4("view", captureViews[i]);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_prefilterMap->getId(), mip);

            s_cubeMesh->vertexArray->bind();
            RenderCommand::clear(RenderCommand::defaultClearBits());
            RenderCommand::renderIndexed(s_cubeMesh->vertexArray);
        }
    }

    framebuffer->unbind();

    if (!s_brdfShader)
        s_brdfShader = Shader::createFromFile("Engine/src/renderer/shader/default/brdf.glsl");

    m_brdfLUTTexture = Texture2D::create(512, 512, GL_RG16F, true, true);
    m_brdfLUTTexture->bind();

    framebuffer->bind();
    renderbuffer = Renderbuffer::create(512, 512, GL_DEPTH_COMPONENT24);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_brdfLUTTexture->getId(), 0);

    glViewport(0, 0, 512, 512);
    s_brdfShader->bind();
    RenderCommand::clear(RenderCommand::defaultClearBits());
    auto mesh = MeshFactory::quadMesh(-1.f, -1.f, 1.f, 1.f);
    RenderCommand::renderIndexed(mesh->vertexArray);

    framebuffer->unbind();
}

Shared<EnvironmentMap> EnvironmentMap::create(const std::string& file)
{
    auto map = createShared<EnvironmentMap>(file);
    return map;
}

}