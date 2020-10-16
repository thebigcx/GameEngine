#include <FGL/core/Logger.h>

#include <cstdlib>

#include <GL/glew.h>

void GLAPIENTRY messageCallback(GLenum source, 
                                GLenum type, 
                                GLuint id, 
                                GLenum severity, 
                                GLsizei length, 
                                const char* message, 
                                const void* userParam)
{
    Logger::out(message);
}

void Logger::init()
{
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(messageCallback, 0);
}

void Logger::err(const std::string& what)
{
    std::cout << "* Runtime error *\n";
    std::cout << what << "\n\n";

    throw std::runtime_error("Error thrown by Logger::err()");
}