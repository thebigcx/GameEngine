#include <core/Application.h>

#include <iostream>

int main(int argc, char** argv)
{
    auto application = Engine::createApplication();

    application->run();

    delete application;

    return 0;
}