#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>

class Window
{
public:
    Window(int width, int height, std::string title);
    ~Window();

    void setActive();

    bool isOpen();

    void clear();

    void display();
    void pollEvents();

    bool isKeyPressed(int key);

    void close();

private:
    GLFWwindow* m_window;

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
};