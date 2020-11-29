#include <platform/GL/GLRendererAPI.h>

void GLAPIENTRY messageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const char* message, const void* userParam)
{
    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:
            std::cout << "[OpenGL Debug HIGH] " << message << "\n";
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            std::cout << "[OpenGL Debug MEDIUM] " << message << "\n";
            break;
        case GL_DEBUG_SEVERITY_LOW:
            std::cout << "[OpenGL Debug LOW] " << message << "\n";
            break;
    }
}

void GLRendererAPI::init()
{
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(messageCallback, 0);
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
    GLenum result;

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
    }

    switch (dst)
    {
        case BlendFunction::Zero: gldst = GL_ZERO; break;
        case BlendFunction::One: gldst = GL_ONE; break;
        case BlendFunction::SourceAlpha: gldst = GL_SRC_ALPHA; break;
        case BlendFunction::DestinationAlpha: gldst = GL_DST_ALPHA; break;
        case BlendFunction::OneMinusSourceAlpha: gldst = GL_ONE_MINUS_SRC_ALPHA; break;
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
    }

    glDrawElements(GL_TRIANGLES, count, type, (void*)(offset * sizeof(uint32_t)));
}