#include "HUD.h"
#include <sstream>

HUD::HUD(sf::Vector2f position, sf::Vector2f size, const sf::Font& font) {
    background.setSize(size);
    background.setPosition(position);
    background.setFillColor(sf::Color(50, 50, 50));

    scoreText.setFont(font);
    scoreText.setCharacterSize(22);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(position.x + 20, position.y + 10);

    livesText.setFont(font);
    livesText.setCharacterSize(22);
    livesText.setFillColor(sf::Color::White);
    livesText.setPosition(position.x + 220, position.y + 10);

    timeText.setFont(font);
    timeText.setCharacterSize(22);
    timeText.setFillColor(sf::Color::White);
    timeText.setPosition(position.x + 420, position.y + 10);
}

void HUD::setScore(int score) {
    scoreText.setString("Score: " + std::to_string(score));
}

void HUD::setLives(int lives) {
    livesText.setString("Lives: " + std::to_string(lives));
}

void HUD::setTime(float seconds) {
    std::stringstream ss;
    ss << "Time: " << static_cast<int>(seconds);
    timeText.setString(ss.str());
}

void HUD::draw(sf::RenderWindow& window) {
    window.draw(background);
    window.draw(scoreText);
    window.draw(livesText);
    window.draw(timeText);
}
