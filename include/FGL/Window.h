#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>

#include "Image.h"

class Window
{
public:
    Window(int width, int height, std::string title);
    ~Window();

    void setActive();

    bool isOpen();

    void clear();

    void display() const;
    void pollEvents();

    bool isKeyPressed(int key);

    void close();

    void setTitle(const std::string& title);

    void makeCurrentContext() const;

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