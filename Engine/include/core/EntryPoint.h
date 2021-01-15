#include <core/Application.h>
#include <core/Core.h>

#include <iostream>

extern Engine::Application* Engine::createApplication();

int main(int argc, char** argv)
{
    BEGIN_PROFILE_SESSION("Startup");
    auto application = Engine::createApplication();
    END_PROFILE_SESSION();

    BEGIN_PROFILE_SESSION("Runtime");
    application->run();
    END_PROFILE_SESSION();

    BEGIN_PROFILE_SESSION("Shutdown");
    delete application;
    END_PROFILE_SESSION();

    return 0;
}