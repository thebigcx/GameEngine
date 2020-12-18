#include <platform/GL/GLRendererAPI.h>
#include <core/Logger.h>

void GLAPIENTRY messageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const char* message, const void* userParam)
{
    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:
            Logger::getCoreLogger()->error("[OpenGL Debug HIGH] %s", message);
            //abort();
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            Logger::getCoreLogger()->error("[OpenGL Debug MEDIUM] %s", message);
            break;
        case GL_DEBUG_SEVERITY_LOW:
            Logger::getCoreLogger()->error("[OpenGL Debug LOW] %s", message);
            break;
    }
}

void GLRendererAPI::init()
{
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(messageCallback, 0);

    // Capabilities
    int maxTextureUnits;
    glGetIntegerv(GL_MAX_TEXTURE_UNITS, &maxTextureUnits);
    m_capabilities.maxTextureUnits = maxTextureUnits;

    int maxTextureSize;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
    m_capabilities.maxTextureSize = maxTextureSize;

    m_capabilities.version = std::string(reinterpret_cast<const char*>(glGetString(GL_VERSION)));

    int shaderVersion;
    m_capabilities.shaderVersion = std::stoi(reinterpret_cast<const char*>(glGetStringi(GL_SHADING_LANGUAGE_VERSION, 0)));
}

void GLRendererAPI::setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
    glViewport(x, y, width, height);
}

void GLRendererAPI::setClearColor(const math::vec4& color)
{
    glClearColor(color.r, color.g, color.b, color.a);
}

void GLRendererAPI::clear(uint32_t buffer)
{
    GLenum result = 0;

    if (buffer & (uint32_t)RendererBufferType::Color)
        result |= GL_COLOR_BUFFER_BIT;

    if (buffer & (uint32_t)RendererBufferType::Depth)
        result |= GL_DEPTH_BUFFER_BIT;

    if (buffer & (uint32_t)RendererBufferType::Stencil)
        result |= GL_STENCIL_BUFFER_BIT;

    glClear(result);
}

void GLRendererAPI::setDepthTesting(bool enabled)
{
    if (enabled)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);
}

void GLRendererAPI::setBlend(bool enabled)
{
    if (enabled)
        glEnable(GL_BLEND);
    else
        glDisable(GL_BLEND);
}

void GLRendererAPI::setBlendFunction(BlendFunction src, BlendFunction dst)
{
    GLenum glsrc, gldst;

    switch (src)
    {
        case BlendFunction::Zero: glsrc = GL_ZERO; break;
        case BlendFunction::One: glsrc = GL_ONE; break;
        case BlendFunction::SourceAlpha: glsrc = GL_SRC_ALPHA; break;
        case BlendFunction::DestinationAlpha: glsrc = GL_DST_ALPHA; break;
        case BlendFunction::OneMinusSourceAlpha: glsrc = GL_ONE_MINUS_SRC_ALPHA; break;
        default: glsrc = GL_ZERO; break;
    }

    switch (dst)
    {
        case BlendFunction::Zero: gldst = GL_ZERO; break;
        case BlendFunction::One: gldst = GL_ONE; break;
        case BlendFunction::SourceAlpha: gldst = GL_SRC_ALPHA; break;
        case BlendFunction::DestinationAlpha: gldst = GL_DST_ALPHA; break;
        case BlendFunction::OneMinusSourceAlpha: gldst = GL_ONE_MINUS_SRC_ALPHA; break;
        default: gldst = GL_ZERO; break;
    }

    glBlendFunc(glsrc, gldst);
}

void GLRendererAPI::renderIndexed(Shared<VertexArray> array, uint32_t count, uint32_t offset)
{
    if (count == 0)
    {
        count = array->getIndexBuffer()->getCount();
    }

    uint32_t type;
    switch (array->getIndexBuffer()->getDataType())
    {
        case IndexDataType::UInt8:  type = GL_UNSIGNED_BYTE;  break;
        case IndexDataType::UInt16: type = GL_UNSIGNED_SHORT; break;
        case IndexDataType::UInt32: type = GL_UNSIGNED_INT;   break;
        default: type = GL_UNSIGNED_INT; break;
    }

    glDrawElements(GL_TRIANGLES, count, type, (void*)(offset * sizeof(uint32_t)));
}