#include "Wall.h"
#include "Player.h"

Wall::Wall(const sf::Vector2f& position, float tileSize) {
    shape.setSize({ tileSize, tileSize }); 
    shape.setFillColor(sf::Color::Blue); 
    shape.setPosition(position);
}

void Wall::draw(sf::RenderWindow& window) const {
    window.draw(shape);
}

sf::FloatRect Wall::getBounds() const {
    return shape.getGlobalBounds();
}