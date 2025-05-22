#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.h"

class Wall : public GameObject {
public:
    Wall(const sf::Vector2f& position, float tileSize);
    void draw(sf::RenderWindow& window) const override;
    sf::FloatRect getBounds() const override;

private:
    sf::RectangleShape shape;
};
