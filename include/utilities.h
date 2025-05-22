#pragma once

// ??????
constexpr float PLAYER_SPEED = 250.0f;
constexpr float ENEMY_SPEED = 600.0f;
constexpr float SMART_ENEMY_SPEED = 250.0f;

constexpr float HUD_TOP_Y = 960.0f;

constexpr int FIRST_LEVEL = 0;

// ????? (?? ????)
#include <SFML/Graphics.hpp>
const sf::Color PLAYER_COLOR = sf::Color::Green;
const sf::Color TRAIL_COLOR = sf::Color(255, 105, 180); // pink
const sf::Color WALL_COLOR = sf::Color::Blue;
