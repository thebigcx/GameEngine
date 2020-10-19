#pragma once

#include <vector>

#include "Window.h"
#include "Layer.h"
#include "../events/EventStack.h"

// Singleton
class Application
{
public:
    Application();

    void run();

    void addLayer(Layer* layer);
    
    static Application& get();

    Window& getWindow();

    void onWindowResize(int width, int height);

    inline EventStack& getEventStack()
    {
        return m_eventStack;
    }

private:
    static Application* m_instance;

    Window m_window;

    std::vector<Layer*> m_layers;

    EventStack m_eventStack;
};

Application* createApplication();