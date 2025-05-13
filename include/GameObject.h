#pragma once
#include <SFML/Graphics.hpp>

class Player;
class Enemy;
class Wall; // new

class GameObject {
public:
    virtual ~GameObject() {}

    virtual void collideWith(GameObject& other) = 0;
    virtual void draw(sf::RenderWindow& window) const = 0;



    virtual void collideWithPlayer(Player& player) {}
    virtual void collideWithEnemy(Enemy& enemy) {}
    virtual void collideWithWall(Wall& wall) {} // new

    virtual sf::FloatRect getBounds() const = 0;
};
