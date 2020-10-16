#pragma once

#include "Window.h"

class Application
{
public:
    Application();

    void run();
    
    static Application& get();

    void setActiveWindow(Window& window);

    const Window& getActiveWindow() const;

private:
    static Application* m_instance;

    Window* m_window;
};