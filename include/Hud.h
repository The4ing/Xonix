#pragma once
#include <SFML/Graphics.hpp>

class HUD {
public:
    HUD(sf::Vector2f position, sf::Vector2f size, const sf::Font& font);

    void setScore(int score);
    void setLives(int lives);
    void setTime(float seconds);

    void draw(sf::RenderWindow& window);

private:
    sf::RectangleShape background;
    sf::Text scoreText;
    sf::Text livesText;
    sf::Text timeText;
};
