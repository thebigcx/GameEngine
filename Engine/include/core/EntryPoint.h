#include <core/Application.h>

#include <iostream>

extern Engine::Application* Engine::createApplication();

int main(int argc, char** argv)
{
    auto application = Engine::createApplication();

    application->run();

    delete application;

    return 0;
}