#include <core/Application.h>

#include "EditorLayer.h"

namespace Engine
{

class EditorApp : public Application
{
public:
    EditorApp()
        : Application()
    {
        addLayer(new Engine::EditorLayer());
    }
};

Application* createApplication()
{
    return new EditorApp();
}

}