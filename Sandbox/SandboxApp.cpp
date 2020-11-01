#include <core/Application.h>

#include "Sandbox.h"

class SandboxApp : public Application
{
public:
    SandboxApp()
        : Application()
    {
        addLayer(new Sandbox());
    }
};

Application* createApplication()
{
    return new SandboxApp();
}