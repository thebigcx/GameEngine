#include <FGL/core/Logger.h>

#include <cstdlib>

#include <GL/glew.h>

void GLAPIENTRY messageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const char* message, const void* userParam)
{
    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:
            Logger::errf("[OpenGL Debug HIGH] %s", message);
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            Logger::outf("[OpenGL Debug MEDIUM] %s", message);
            break;
        case GL_DEBUG_SEVERITY_LOW:
            Logger::outf("[OpenGL Debug LOW] %s", message);
            break;
    }
}

void Logger::init()
{
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(messageCallback, 0);
}