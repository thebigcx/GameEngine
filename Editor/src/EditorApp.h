#pragma once

#include <core/Game.h>

#include "EditorLayer.h"

namespace Engine
{

class EditorApp : public Game
{
public:
    EditorApp()
        : Game()
    {
        
    }

    void initialize() override
    {
        addLayer(new Engine::EditorLayer());
    }
};

}