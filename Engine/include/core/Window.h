#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>

#include <util/Image.h>

class Window
{
public:
    Window() {}

    Window(int width, int height, const std::string& title);
    ~Window();

    bool isOpen();

    void pollEvents();

    inline GLFWwindow* getNative() const
    {
        return m_window;
    }

    Vector2i getSize() const;

    void close();

    void setTitle(const std::string& title);

    void setIcon(const std::string& iconPath);
    void setIcon(const Image& image);

    void setSize(const Vector2u& size);

    void minimize();
    void maximize();
    void restore();

    void setOpacity(float opacity);

private:
    GLFWwindow* m_window;

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

};