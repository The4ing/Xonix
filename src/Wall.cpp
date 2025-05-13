#include "Wall.h"
#include "Player.h"

Wall::Wall(const sf::Vector2f& position, float tileSize) {
    shape.setFillColor(sf::Color(100, 100, 100)); // gray
    shape.setPosition(position);
}

void Wall::draw(sf::RenderWindow& window) const {
    window.draw(shape);
}

sf::FloatRect Wall::getBounds() const {
    return shape.getGlobalBounds();
}

void Wall::collideWith(GameObject& other) {
    other.collideWithWall(*this);
}

void Wall::collideWithPlayer(Player& player) {
    //std::cout << "Player collided with Wall!" << std::endl;
    // you can stop movement, bounce back, etc.
}

void Wall::collideWithSmartEnemy(SmartEnemy& smartEnemy) {
    // handle logic (e.g., lose life, reset, etc.)
}
