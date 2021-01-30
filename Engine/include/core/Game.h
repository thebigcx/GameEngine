#pragma once

#include <vector>

#include <events/Event.h>
#include <core/Core.h>

namespace Engine
{

class Window;
class Layer;
class ImGuiLayer;
class WindowResizeEvent;
class WindowCloseEvent;

// Singleton
class Game
{
public:
    Game();
    ~Game();

    Game(const Game& app) = delete;
    Game(Game&& app) = delete;

    int run();
    void addLayer(Layer* layer);
    bool onWindowResize(WindowResizeEvent& event);
    bool onWindowClose(WindowCloseEvent& event);
    void quit();
    void setCursorEnabled(bool enabled);

    void shutdown();

    virtual void initialize() {}
    virtual void finalize() {}

    void onEvent(Event& event);

    inline constexpr ImGuiLayer* getImGuiLayer() const { return m_imguiLayer; }

    Window& getWindow();

    static Game* getInstance();

private:
    static Game* m_instance;

    Owned<Window> m_window;

    std::vector<Layer*> m_layers;
    ImGuiLayer* m_imguiLayer;

    bool m_running = true;

    uint64_t m_frames;
};

}