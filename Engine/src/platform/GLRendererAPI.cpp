#include <platform/GLRendererAPI.h>

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

void GLRendererAPI::setClearColor(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
}

void GLRendererAPI::clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLRendererAPI::renderIndexed(Shared<VertexArray> array)
{
    glDrawElements(GL_TRIANGLES, array->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, 0);
}