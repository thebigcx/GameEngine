#include <core/Application.h>

#include "EditorLayer.h"

class EditorApp : public Application
{
public:
    EditorApp()
        : Application()
    {
        addLayer(new EditorLayer());
    }
};

Application* createApplication()
{
    return new EditorApp();
}