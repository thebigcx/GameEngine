#pragma once

#include <vector>

#include <core/Window.h>
#include <core/Layer.h>
#include <events/Event.h>
#include <desktop/ImGuiLayer.h>

// Singleton
class Application
{
public:
    Application();
    ~Application();

    void run();
    void addLayer(Layer* layer);
    bool onWindowResize(WindowResizeEvent& event);
    bool onWindowClose(WindowCloseEvent& event);
    void quit();
    void setCursorEnabled(bool enabled);

    void onEvent(Event& event);

    Window& getWindow();

    static Application& get();

private:
    static Application* m_instance;

    Unique<Window> m_window;

    std::vector<Layer*> m_layers;
    ImGuiLayer* m_imguiLayer;

    bool m_running = true;
};

Application* createApplication();