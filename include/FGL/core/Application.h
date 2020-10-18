#pragma once

#include "Window.h"

// Singleton
class Application
{
public:
    Application();

    void run();
    
    static Application& get();

    Window& getWindow();

    void onWindowResize(int width, int height);

private:
    static Application* m_instance;

    Window m_window;
};