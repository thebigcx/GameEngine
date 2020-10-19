#pragma once

#include <FGL/core/Application.h>

#include "Sandbox.h"

class SandboxApp : public Application
{
public:
    SandboxApp()
    {
        addLayer(new Sandbox());
    }
};