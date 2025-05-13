#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.h"

class Wall : public GameObject {
public:
    Wall(const sf::Vector2f& position, float tileSize);

    void draw(sf::RenderWindow& window) const override;

    // GameObject overrides
    void collideWith(GameObject& other) override;
    void collideWithPlayer(Player& player) override;
    sf::FloatRect getBounds() const override;
    void collideWithSmartEnemy(SmartEnemy& smartEnemy) override;

private:
    sf::RectangleShape shape;
};
