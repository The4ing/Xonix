#pragma once
#include <SFML/Graphics.hpp>

class Player;
class Enemy;
class Wall; 
class SmartEnemy; 

class GameObject {
public:
    virtual ~GameObject() {}
    virtual void draw(sf::RenderWindow& window) const = 0;
    virtual sf::FloatRect getBounds() const = 0;
};
