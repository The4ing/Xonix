#pragma once
#include <SFML/Graphics.hpp>
#include "LevelGrid.h"
#include "GameObject.h"
#include <iostream>
using namespace std;
constexpr float HUD_TOP_Y = 960.0f;

class Player : public GameObject {
public:
    Player(int startX, int startY, float tileSize);

    void handleInput();
    void update(sf::Time dt);
    void draw(sf::RenderWindow& window) const override;


    void setWindowSize(sf::Vector2f size);
    void setGrid(LevelGrid* grid);
    void stop();

    // Inherited from GameObject
    void collideWith(GameObject& other) override;
    void collideWithWall(Wall& wall) override;
    void collideWithEnemy(class Enemy& enemy) override;
    void collideWithSmartEnemy(SmartEnemy& smartEnemy) override;
    sf::FloatRect getBounds() const override;
    sf::Vector2f getPosition() const;

private:
    TileType getCurrentTile() const; //for debeuging purposes, can be deleted
    std::string tileTypeToString(TileType type);




    //void applyFloodFill();

    sf::Vector2f actualPos;
    sf::RectangleShape shape;
    float tileSize;
    float moveSpeed;
    sf::Vector2i gridDirection;
    sf::Vector2f windowSize;
    LevelGrid* gridRef = nullptr;
};
