#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.h"

class LevelGrid;

class Enemy : public GameObject {
public:
    Enemy(const sf::Vector2f& pos, float speed, float tileSize);

    void update(sf::Time dt, const LevelGrid& level);
    void draw(sf::RenderWindow& window) const override;

    sf::FloatRect getBounds() const override;
    void collideWith(GameObject& other) override;
    void collideWithPlayer(class Player& player) override;
    void collideWithSmartEnemy(SmartEnemy& smartEnemy) override;

    sf::Vector2f getPosition() const;
    sf::Vector2f getDirection() const;

private:
    sf::CircleShape shape;

    sf::Vector2f direction;
    float speed;
    float tileSize;

    void handleCollisionWithWall(const LevelGrid& level);
};
