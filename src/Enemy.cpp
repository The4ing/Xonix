#include "Enemy.h"
#include "LevelGrid.h"
#include "Player.h"
#include <iostream>
#include <cstdlib>

Enemy::Enemy(const sf::Vector2f& pos, float speed, float tileSize)
    : speed(speed), tileSize(tileSize)
{
    shape.setRadius(tileSize / 2.f);
    shape.setOrigin(tileSize / 2.f, tileSize / 2.f); 
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
    sf::Vector2f next = pos + direction * tileSize;

    int currentRow = static_cast<int>((pos.y + tileSize / 2) / tileSize);
    int currentCol = static_cast<int>((pos.x + tileSize / 2) / tileSize);
    int nextRow = static_cast<int>((next.y + tileSize / 2) / tileSize);
    int nextCol = static_cast<int>((next.x + tileSize / 2) / tileSize);

    // Vertical collision
    if (level.isCollidable(nextRow, currentCol)) {
        direction.y *= -1;
    }

    // Horizontal collision
    if (level.isCollidable(currentRow, nextCol)) {
        direction.x *= -1;
    }
}

void Enemy::draw(sf::RenderWindow& window) const {
    window.draw(shape);
}

sf::FloatRect Enemy::getBounds() const {
    return shape.getGlobalBounds();
}

sf::Vector2f Enemy::getPosition() const {
    return shape.getPosition();
}
