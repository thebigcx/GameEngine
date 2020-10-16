#include "FGL/core/Application.h"

#include "FGL/core/Logger.h"

Application* Application::m_instance = nullptr;

Application::Application()
{
    m_instance = this;
}

Application& Application::get()
{
    if (m_instance == nullptr)
    {
        Logger::log("Application instance is undefined!");
    }

    return *m_instance;
}

void Application::setActiveWindow(Window& window)
{
    m_window = &window;
}

const Window& Application::getActiveWindow() const
{
    return *m_window;
}