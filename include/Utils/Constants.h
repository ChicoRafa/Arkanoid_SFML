#pragma once
#include "SFML/System/Vector2.hpp"
/**
 * @brief Constant values used in the game
 */
constexpr char* GAME_TITLE = "ARKANOID";
const float SCREEN_WIDTH = 800.f;
const float SCREEN_HEIGHT = 600.f;
inline const char* TEXT_FONT = "../Data/Fonts/Arcadepix Plus.ttf";
const float SIZE_MULTIPLIER = 1.5f;
const sf::Vector2f GRAVITY(0.0f, 20.0f); // earth gravity is 9.8 m/s^2, but for sake of fun we'll use 20.0f

