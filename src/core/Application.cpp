#include <FGL/core/Application.h>

#include <FGL/core/Logger.h>
#include <FGL/audio/SoundManager.h>
#include <FGL/util/maths/Math.h>
#include <FGL/core/Keyboard.h>

Application* Application::m_instance = nullptr;

Application::Application()
: m_window(1280, 720, "Hello, Fast Game Library!")
{
    m_instance = this;

    Math::Random::initSeed();

    Logger::init();

    SoundManager::init();
}

void Application::run()
{
    while (m_window.isOpen())
    {
        m_window.pollEvents();

        if (Keyboard::isKeyPressed(Keyboard::Key::Escape))
        {
            m_window.close();
        }

        for (auto layer : m_layers)
        {
            layer->update();
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