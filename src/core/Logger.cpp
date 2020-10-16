#include <FGL/core/Logger.h>

#include <cstdlib>

#include <GL/glew.h>

void GLAPIENTRY messageCallback( GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam )
{
    Logger::log(message);
}

void Logger::init()
{
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(messageCallback, 0);
}