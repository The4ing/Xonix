#include "HUD.h"

HUD::HUD(sf::Vector2f position, sf::Vector2f size, const sf::Font& font) {
    // ??? ????
    background.setSize(size);
    background.setPosition(position);
    background.setFillColor(sf::Color(50, 50, 50));

    // ?????? ?????? (Score, Lives, Time)
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

    // ???? ???? ???? (??? ???? ??? ???????)
    areaText.setFont(font);
    areaText.setCharacterSize(18);
    areaText.setFillColor(sf::Color::White);
    areaText.setPosition(position.x + 620, position.y + 8);

    // ????? ?? ???????
    sf::Vector2f barSize(150.0f, 16.0f);
    areaBarBg.setSize(barSize);
    areaBarBg.setPosition(position.x + 620, position.y + 36);
    areaBarBg.setFillColor(sf::Color(100, 100, 100));
    areaBarBg.setOutlineColor(sf::Color::White);
    areaBarBg.setOutlineThickness(1.0f);

    areaBarFill.setSize({ 0.0f, barSize.y });
    areaBarFill.setPosition(areaBarBg.getPosition());
    areaBarFill.setFillColor(sf::Color(100, 250, 50));  // ???? ????? ????
}

void HUD::setScore(int score) {
    scoreText.setString("Score: " + std::to_string(score));
}

void HUD::setLives(int lives) {
    livesText.setString("Lives: " + std::to_string(lives));
}

// HUD.cpp
void HUD::setTime(float seconds) {
    int sec = static_cast<int>(seconds);
    int minutes = sec / 60;
    int secs = sec % 60;

    std::stringstream ss;
    ss << "Time: " << minutes << ":"
        << (secs < 10 ? "0" : "") << secs;
    timeText.setString(ss.str());
}


void HUD::setAreaPercent(float percent) {
    // 1) ????? ????
    std::stringstream ss;
    ss << static_cast<int>(percent) << "% Closed";
    areaText.setString(ss.str());

    // 2) ????? ???? ?-fill ?? ???
    float widthBg = areaBarBg.getSize().x;
    areaBarFill.setSize({ widthBg * (percent / 100.0f),
                          areaBarBg.getSize().y });

    // 3) (?????????) ????? ???? ????? ?????
    if (percent < 50.f) {
        areaBarFill.setFillColor(sf::Color(250, 100, 100)); // ???? ?? 50%
    }
    else {
        areaBarFill.setFillColor(sf::Color(100, 250, 100)); // ???? ??? 50%
    }
}

void HUD::draw(sf::RenderWindow& window) {
    window.draw(background);

    // ??????
    window.draw(scoreText);
    window.draw(livesText);
    window.draw(timeText);
    window.draw(areaText);

    // ?? ???????
    window.draw(areaBarBg);
    window.draw(areaBarFill);
}
