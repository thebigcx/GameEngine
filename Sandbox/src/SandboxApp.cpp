#include <core/Application.h>
#include <core/EntryPoint.h>

#include "Sandbox.h"

class SandboxApp : public Engine::Application
{
public:
    SandboxApp()
        : Engine::Application()
    {
        addLayer(new Sandbox());
    }
};

Engine::Application* Engine::createApplication()
{
    return new SandboxApp();
}