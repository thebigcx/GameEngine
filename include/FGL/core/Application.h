#pragma once

#include <vector>

#include "Window.h"
#include "Layer.h"

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

private:
    static Application* m_instance;

    Window m_window;

    std::vector<Layer*> m_layers;
};

Application* createApplication();