#pragma once

#include <string>
#include <functional>

#include <util/Image.h>
#include <maths/vector/vec2.h>
#include <events/Event.h>

struct GLFWwindow;

namespace Engine
{

struct WindowData
{
    std::function<void(Event&)> eventCallback;
};

class RenderingContext;

class Window
{
private:
    friend class Game;

public:
    Window(uint32_t width, uint32_t height, const std::string& title);
    ~Window();

    bool isOpen();

    void setEventCallback(const std::function<void(Event&)>& function);

    void pollEvents();

    void onUpdate();

    inline GLFWwindow* getNative() const
    {
        return m_window;
    }

    inline constexpr uint32_t getWidth() const noexcept { return m_height; }
    inline constexpr uint32_t getHeight() const noexcept { return m_width; }
    inline math::uvec2 getSize() const { return math::uvec2(m_width, m_height); }

    void close();

    void setTitle(const std::string& title);

    void setIcon(const std::string& iconPath);
    void setIcon(const Reference<Image>& image);

    void setSize(const math::uvec2& size);

    void minimize();
    void maximize();
    void restore();

    void setOpacity(float opacity);

    static Owned<Window> create(uint32_t width, uint32_t height, const std::string& title);

private:
    GLFWwindow* m_window;

    Owned<RenderingContext> m_context;

    WindowData m_data;

    uint32_t m_width = 0, m_height = 0;
};

}