#include <core/Application.h>

#include <core/Logger.h>
#include <audio/SoundEngine.h>
#include <util/maths/Math.h>
#include <core/Keyboard.h>
#include <events/EventDispatcher.h>
#include <renderer/Renderer.h>

Application* Application::m_instance = nullptr;

Application::Application()
: m_window(1280, 720, "Hello, Fast Game Library!")
{
    m_instance = this;

    Math::Random::initSeed();

    Logger::init();

    SoundEngine::init();

    EventDispatcher::setupCallbacks();

    Renderer::init();
}

void Application::run()
{
    while (m_window.isOpen())
    {
        m_window.pollEvents();
        
        while (!m_eventStack.isEmpty())
        {
            auto event = m_eventStack.getEvent();

            for (auto layer : m_layers)
            {
                layer->handleEvent(event);
            }

            m_eventStack.pop();
        }

        if (Keyboard::isKeyPressed(Keyboard::Key::Escape))
        {
            m_window.close();
        }

        for (auto layer : m_layers)
        {
            layer->update(1);
        }
    }
}

void Application::addLayer(Layer* layer)
{
    m_layers.push_back(layer);
}

Application& Application::get()
{
    if (m_instance == nullptr)
    {
        Logger::err("Application instance is undefined!");
    }

    return *m_instance;
}

void Application::onWindowResize(int width, int height)
{
    glViewport(0, 0, width, height);
}

Window& Application::getWindow()
{
    return m_window;
}