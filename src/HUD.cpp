#include "HUD.h"

HUD::HUD(sf::Vector2f position, sf::Vector2f size, const sf::Font& font) {

    background.setSize(size);
    background.setPosition(position);
    background.setFillColor(sf::Color(50, 50, 50));


    float hudX = position.x;
    float hudY = position.y;
    float hudW = size.x;
    float hudH = size.y;

    auto centerText = [&](sf::Text& text, float xFrac, float yFrac) {
        sf::FloatRect bb = text.getLocalBounds();
        text.setOrigin(bb.width / 2, bb.height / 2);
        text.setPosition(hudX + hudW * xFrac,
            hudY + hudH * yFrac);
        };

    // Lives Text
    livesText.setFont(font);
    livesText.setCharacterSize(59);
    livesText.setFillColor(sf::Color::White);
    centerText(livesText, 0.25f, 0.2f);

    // Time Text
    timeText.setFont(font);
    timeText.setCharacterSize(59);
    timeText.setFillColor(sf::Color::White);
    centerText(timeText, 0.5f, 0.2f);

    // Area % Text
    areaText.setFont(font);
    areaText.setCharacterSize(57);
    areaText.setFillColor(sf::Color::White);
    centerText(areaText, 0.65f, 0.1f);

    // Area Bar
    sf::Vector2f barSize(150.0f, 20.0f);
    areaBarBg.setSize(barSize);
    areaBarBg.setFillColor(sf::Color(100, 100, 100));
    areaBarBg.setOutlineColor(sf::Color::White);
    areaBarBg.setOutlineThickness(1.0f);

    // place under areaText
    sf::Vector2f areaPos = areaText.getPosition();
    float barX = areaPos.x - barSize.x / 2 + 20;
    float barY = areaPos.y + 70.0f; // small padding below text
    areaBarBg.setPosition(barX, barY);
    areaBarFill.setSize({ 0.0f, barSize.y });
    areaBarFill.setPosition(barX, barY);
    areaBarFill.setFillColor(sf::Color(100, 250, 50));
}

void HUD::setLives(int lives) {
    livesText.setString("Lives: " + std::to_string(lives));
}

void HUD::setTime(float seconds) {
    int sec = static_cast<int>(seconds);
    int minutes = sec / 60;
    int secs = sec % 60;
    std::stringstream ss;
    ss << minutes << ":" << (secs < 10 ? "0" : "") << secs;
    timeText.setString(ss.str());
}

void HUD::setAreaPercent(float percent) {
    areaText.setString(std::to_string(static_cast<int>(percent)) + "%");
    // adjust bar fill width
    float widthBg = areaBarBg.getSize().x;
    areaBarFill.setSize({ widthBg * (percent / 100.0f), areaBarBg.getSize().y });
    // color feedback
    if (percent < 50.f)
        areaBarFill.setFillColor(sf::Color(250, 100, 100));
    else
        areaBarFill.setFillColor(sf::Color(100, 250, 100));
}

void HUD::draw(sf::RenderWindow& window) {
    window.draw(background);
    window.draw(livesText);
    window.draw(timeText);
    window.draw(areaBarBg);
    window.draw(areaBarFill);
    window.draw(areaText);
}

void HUD::setAreaProgress(float closedPercent, float requiredPercent) {
    std::stringstream ss;
    ss << static_cast<int>(closedPercent) << " / " << static_cast<int>(requiredPercent) << " %";
    areaText.setString(ss.str());
}
