#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <functional>

#include <util/Image.h>
#include <maths/vector/vec2.h>
#include <renderer/RenderingContext.h>
#include <events/Event.h>

struct WindowData
{
    std::function<void(Event&)> eventCallback;
};

class Window
{
public:
    Window(int width, int height, const std::string& title);
    ~Window();

    bool isOpen();

    void setEventCallback(const std::function<void(Event&)>& function);

    void pollEvents();

    void onUpdate();

    inline GLFWwindow* getNative() const
    {
        return m_window;
    }

    int getWidth() const;
    int getHeight() const;
    math::ivec2 getSize() const;

    void close();

    void setTitle(const std::string& title);

    void setIcon(const std::string& iconPath);
    void setIcon(const Image& image);

    void setSize(const math::uvec2& size);

    void minimize();
    void maximize();
    void restore();

    void setOpacity(float opacity);

    static Unique<Window> create(int width, int height, const std::string& title);

private:
    GLFWwindow* m_window;

    Unique<RenderingContext> m_context;

    void init(int width, int height, const std::string& title);

    WindowData m_data;
};