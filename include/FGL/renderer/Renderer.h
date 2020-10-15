#pragma once

#include "../Window.h"
#include "../util/Color.h"

class Renderer
{
    public:
        static void init();

        static void clear(const Color& color);
        static void display();

        static void setContext(const Window& window);

    private:
        static const Window* m_currentContext;
};