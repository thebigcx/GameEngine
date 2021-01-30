#pragma once

#include <renderer/RenderingContext.h>

struct GLFWwindow;

namespace Engine
{

class GLContext : public RenderingContext
{
public:
    GLContext(GLFWwindow* window);

    void init() override;
    void swapBuffers() override;
    void vsync(bool enabled) override;

private:
    GLFWwindow* m_window;
};

}