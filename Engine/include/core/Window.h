#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <functional>

#include <util/ImageLoader.h>
#include <maths/vector/vec2.h>
#include <renderer/RenderingContext.h>
#include <events/Event.h>

namespace Engine
{

struct WindowData
{
    std::function<void(Event&)> eventCallback;
};

class Window
{
private:
    friend class Application;

public:
    Window(unsigned int width, unsigned int height, const std::string& title);
    ~Window();

    bool isOpen();

    void setEventCallback(const std::function<void(Event&)>& function);

    void pollEvents();

    void onUpdate();

    inline GLFWwindow* getNative() const
    {
        return m_window;
    }

    inline constexpr unsigned int getWidth() const noexcept { return m_height; }
    inline constexpr unsigned int getHeight() const noexcept { return m_width; }
    inline math::uvec2 getSize() const { return math::uvec2(m_width, m_height); }

    void close();

    void setTitle(const std::string& title);

    void setIcon(const std::string& iconPath);
    void setIcon(const Image& image);

    void setSize(const math::uvec2& size);

    void minimize();
    void maximize();
    void restore();

    void setOpacity(float opacity);

    static Unique<Window> create(unsigned int width, unsigned height, const std::string& title);

private:
    GLFWwindow* m_window;

    Unique<RenderingContext> m_context;

    void init(unsigned int width, unsigned int height, const std::string& title);

    WindowData m_data;

    unsigned int m_width = 0, m_height = 0;
};

}