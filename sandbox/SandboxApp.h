#pragma once

#include <core/Application.h>

#include "Sandbox.h"

class SandboxApp : public Application
{
public:
    SandboxApp()
    {
        addLayer(new Sandbox());
    }
};