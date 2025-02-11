#pragma once

class Game
{
  public:
    /// Updates the game state  
    /// Parameters:
    /// - dt: The time since the last tick in milliseconds
    void tick( float dt );
    float angleDegrees = 0;
};
