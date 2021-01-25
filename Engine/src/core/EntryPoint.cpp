#include <core/Game.h>

int main(int argc, char** argv)
{
    Engine::Game* game = Engine::Game::getInstance();
    return game->run();
}