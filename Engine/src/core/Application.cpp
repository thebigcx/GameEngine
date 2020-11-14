#include <core/Application.h>

#include <core/Console.h>
#include <sound/SoundEngine.h>
#include <math/math.h>
#include <core/Input.h>
#include <events/EventDispatcher.h>
#include <renderer/Renderer2D.h>
#include <util/Time.h>
#include <renderer/RenderCommand.h>

Application* Application::m_instance = nullptr;

Application::Application()
{
    m_instance = this;

    m_window = Window::create(1280, 720, "Application");

    math::Random::initSeed();
    RenderCommand::init();
    SoundEngine::init();
    EventDispatcher::setupCallbacks();
    Renderer2D::init();
    Time::init();
}

void Application::run()
{
    while (m_window->isOpen())
    {
        Time::update();

        m_window->pollEvents();
        
        while (!m_eventStack.isEmpty())
        {
            auto event = m_eventStack.getEvent();

            for (auto layer : m_layers)
            {
                layer->handleEvent(event);
            }

            m_eventStack.pop();
        }

        for (auto layer : m_layers)
        {
            layer->update();
        }

        m_window->onUpdate();
    }
}

void Application::quit()
{
    m_window->close();
}

void Application::addLayer(Layer* layer)
{
    m_layers.push_back(layer);
}

Application& Application::get()
{
    if (m_instance == nullptr)
    {
        Console::err("Application instance is undefined!");
    }

    return *m_instance;
}

void Application::onWindowResize(int width, int height)
{
    glViewport(0, 0, width, height);
}

Window& Application::getWindow()
{
    return *m_window;
}