#include <core/Application.h>

#include <core/Console.h>
#include <sound/SoundEngine.h>
#include <maths/math.h>
#include <core/Input.h>
#include <events/EventDispatcher.h>
#include <renderer/Renderer2D.h>
#include <renderer/Renderer3D.h>
#include <renderer/Renderer.h>
#include <util/Time.h>
#include <renderer/RenderCommand.h>

Application* Application::m_instance = nullptr;

Application::Application()
{
    m_instance = this;

    m_window = Window::create(1280, 720, "Application");

    math::random::initSeed();
    SoundEngine::init();
    EventManager::setupCallbacks();
    Renderer::init();
    Time::init();
}

void Application::run()
{
    while (m_window->isOpen())
    {
        Time::update();

        m_window->pollEvents();

        for (auto layer : m_layers)
        {
            layer->update();
        }

        m_window->onUpdate();
    }
}

void Application::onEvent(Event& event)
{
    EventDispatcher dispatcher(event);
    dispatcher.dispatch<WindowResizedEvent>(BIND_EVENT_FN(Application::onWindowResize));

    for (auto& layer : m_layers)
    {
        layer->handleEvent(event);
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

void Application::onWindowResize(WindowResizedEvent& event)
{
    glViewport(0, 0, event.getWidth(), event.getHeight());

    Renderer::windowResize(event);
}

Window& Application::getWindow()
{
    return *m_window;
}

void Application::setCursorEnabled(bool enabled)
{
    uint32_t glfwEnabled = enabled ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED;

    glfwSetInputMode(m_window->getNative(), GLFW_CURSOR, glfwEnabled);
}