#include "game.hpp"
#include <cmath>

void Game::tick(float dt)
{
    float degreesPerMs = 360.0 / 1000; // one spin per second
    angleDegrees = std::fmod(angleDegrees + degreesPerMs * dt, 360);
}
