#include "Enemy.h"
#include "LevelGrid.h"
#include "Player.h"
#include <iostream>
#include <cstdlib>

Enemy::Enemy(const sf::Vector2f& pos, float speed, float tileSize)
    : speed(speed), tileSize(tileSize)
{
    shape.setRadius(tileSize / 2.f);
    shape.setOrigin(tileSize / 2.f, tileSize / 2.f); // center origin for correct collision
    shape.setPosition(pos);
    shape.setFillColor(sf::Color::Red);


    int dx = (rand() % 2 == 0) ? 1 : -1;
    int dy = (rand() % 2 == 0) ? 1 : -1;
    direction = sf::Vector2f(dx, dy);
}

void Enemy::update(sf::Time dt, const LevelGrid& level) {
    shape.move(direction * speed * dt.asSeconds());
    handleCollisionWithWall(level);
}

void Enemy::handleCollisionWithWall(const LevelGrid& level) {
    sf::Vector2f pos = shape.getPosition();

    // Check horizontal and vertical positions separately
    sf::Vector2f next = pos + direction * tileSize   ;

    int currentRow = static_cast<int>((pos.y + tileSize / 2) / tileSize);
    int currentCol = static_cast<int>((pos.x + tileSize / 2) / tileSize);

    int nextRow = static_cast<int>((next.y + tileSize / 2) / tileSize);
    int nextCol = static_cast<int>((next.x + tileSize / 2) / tileSize);

    // Separate checks for Y and X
    // Check vertical collision (Y axis)
    
    
    if (level.get(nextRow, currentCol) == TileType::Wall || level.get(nextRow, currentCol) == TileType::Filled
        || level.get(nextRow, currentCol) == TileType::PlayerPath) {
        direction.y *= -1; // reverse vertical
    }

    // Check horizontal collision (X axis)
    
    if (level.get(currentRow, nextCol) == TileType::Wall || level.get(currentRow, nextCol) == TileType::Filled
        || level.get(currentRow, nextCol) == TileType::PlayerPath) {
        direction.x *= -1; // reverse horizontal
    }
}


void Enemy::draw(sf::RenderWindow& window) const {
    window.draw(shape);
}

sf::FloatRect Enemy::getBounds() const {
    return shape.getGlobalBounds();
}


void Enemy::collideWith(GameObject& other) {
    other.collideWithEnemy(*this);
}

void Enemy::collideWithPlayer(Player& player) {
    std::cout << "Enemy hit player!" << std::endl;
    // Handle loss of life here if needed
}

void Enemy::collideWithSmartEnemy(SmartEnemy& smartEnemy) {
    // handle logic (e.g., lose life, reset, etc.)
}

sf::Vector2f Enemy::getPosition() const {
    return shape.getPosition();
}

sf::Vector2f Enemy::getDirection() const {
    return direction;
}
