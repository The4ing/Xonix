#pragma once
#include <SFML/Graphics.hpp>
#include <sstream>

class HUD {
public:
    HUD(sf::Vector2f position, sf::Vector2f size, const sf::Font& font);
    void setLives(int lives);
    void setTime(float seconds);
    void setAreaPercent(float percent);
    void draw(sf::RenderWindow& window);
    void setAreaProgress(float closedPercent, float requiredPercent);


private:
    sf::RectangleShape background;
    sf::Text scoreText;
    sf::Text livesText;
    sf::Text timeText;
    sf::Text areaText;
    sf::RectangleShape areaBarBg;
    sf::RectangleShape areaBarFill;
};
