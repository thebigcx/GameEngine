#pragma once

#include <vector>

#include <core/Window.h>
#include <core/Layer.h>
#include <events/EventStack.h>

// Singleton
class Application
{
public:
    Application();

    void run();
    void addLayer(Layer* layer);
    void onWindowResize(int width, int height);
    void quit();
    void setCursorEnabled(bool enabled);

    Window& getWindow();
    
    inline EventStack& getEventStack()
    {
        return m_eventStack;
    }

    static Application& get();

private:
    static Application* m_instance;

    Unique<Window> m_window;

    std::vector<Layer*> m_layers;

    EventStack m_eventStack;
};

Application* createApplication();