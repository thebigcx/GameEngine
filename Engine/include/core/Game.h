#pragma once

#include <vector>

#include <core/Window.h>
#include <core/Layer.h>
#include <events/Event.h>
#include <desktop/ImGuiLayer.h>

int main(int argc, char** argv);

namespace Engine
{

// Singleton
class Game
{
public:
    Game();
    ~Game();

    Game(const Game& app) = delete;
    Game(Game&& app) = delete;

    void run();
    void addLayer(Layer* layer);
    bool onWindowResize(WindowResizeEvent& event);
    bool onWindowClose(WindowCloseEvent& event);
    void quit();
    void setCursorEnabled(bool enabled);

    void onEvent(Event& event);

    inline constexpr ImGuiLayer* getImGuiLayer() const { return m_imguiLayer; }

    Window& getWindow();

    static Game& getInstance();

private:
    static Game* m_instance;

    friend int ::main(int argc, char** argv);

    Unique<Window> m_window;

    std::vector<Layer*> m_layers;
    ImGuiLayer* m_imguiLayer;

    bool m_running = true;

    uint64_t m_frames;
};

Game* createGame();

}