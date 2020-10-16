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

private:
    static Application* m_instance;

    Window m_window;
};