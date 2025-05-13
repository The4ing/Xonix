#pragma once
#include <SFML/Graphics.hpp>
#include <queue>
#include <unordered_map>
#include "GameObject.h"
#include "TileType.h"
#include "LevelGrid.h"

class Player;

class SmartEnemy : public GameObject {
public:
    SmartEnemy(const sf::Vector2f& startPos, float tileSize);

    void update(sf::Time dt, const LevelGrid& grid, const Player& player);
    void draw(sf::RenderWindow& window) const override;

    sf::FloatRect getBounds() const override;
    void collideWith(GameObject& other) override;
    void collideWithPlayer(Player& player) override;

private:
    sf::CircleShape shape;
    float tileSize;
    float moveSpeed = 100.f;
    sf::Clock pathClock;

    std::queue<sf::Vector2i> path;

    void findPath(const LevelGrid& grid, const Player& player);
    void moveAlongPath(sf::Time dt);
};
