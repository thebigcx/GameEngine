#include "FGL/core/Application.h"

#include "FGL/core/Logger.h"

Application* Application::m_instance = nullptr;

Application::Application()
: m_window(1280, 720, "Hello, Fast Game Library!")
{
    m_instance = this;
    Logger::init();
}

Application& Application::get()
{
    if (m_instance == nullptr)
    {
        Logger::err("Application instance is undefined!");
    }

    return *m_instance;
}

Window& Application::getWindow()
{
    return m_window;
}