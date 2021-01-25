#include <core/Game.h>
#include <core/EntryPoint.h>

#include "EditorLayer.h"

namespace Engine
{

class EditorApp : public Game
{
public:
    EditorApp()
        : Game()
    {
        addLayer(new Engine::EditorLayer());
    }
};

Game* createGame()
{
    return new EditorApp();
}

}
