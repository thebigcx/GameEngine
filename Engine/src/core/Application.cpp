#include <core/Application.h>

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
    if (m_instance != nullptr)
    {
        Logger::getCoreLogger()->critical("Multiple instances of Application class!");
    }

    m_instance = this;

    m_window = Window::create(1280, 720, "Application");
    m_window->setEventCallback(BIND_EVENT_FN(Application::onEvent));

    math::random::init_seed();
    SoundEngine::init();
    Renderer::init();
    Time::init();

    m_imguiLayer = new ImGuiLayer();
    m_imguiLayer->onAttach();
    m_layers.emplace_back(m_imguiLayer);
}

Application::~Application()
{
    for (auto& layer : m_layers)
    {
        layer->onDetach();
    }
    m_window->close();
    Renderer::shutdown();
}

void Application::run()
{
    while (m_running)
    {
        Time::update();

        m_window->pollEvents();

        for (auto layer : m_layers)
        {
            layer->onUpdate(Time::getDelta());
        }

        m_imguiLayer->begin();
        for (auto layer : m_layers)
        {
            layer->onImGuiRender();
        }
        m_imguiLayer->end();

        m_window->onUpdate();
    }
}

void Application::onEvent(Event& event)
{
    EventDispatcher dispatcher(event);
    dispatcher.dispatch<WindowResizeEvent>(BIND_EVENT_FN(Application::onWindowResize));
    dispatcher.dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::onWindowClose));

    for (auto& layer : m_layers)
    {
        if (event.handled)
        {
            break;
        }
        layer->onEvent(event);
    }
}

void Application::addLayer(Layer* layer)
{
    m_layers.push_back(layer);
    layer->onAttach();
}

Application& Application::get()
{
    if (m_instance == nullptr)
    {
        Logger::getCoreLogger()->error("Application instance is undefined!");
    }

    return *m_instance;
}

void Application::quit()
{
    m_running = false;
}

bool Application::onWindowResize(WindowResizeEvent& event)
{
    Renderer::windowResize(event);
    return false;
}

bool Application::onWindowClose(WindowCloseEvent& event)
{
    m_running = false;
    return true;
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