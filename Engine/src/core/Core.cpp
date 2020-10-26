#include <core/Application.h>

#include <iostream>

int main()
{
    auto application = createApplication();

    application->run();

    delete application;

    return 0;
}