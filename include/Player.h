
#pragma once
#include <SFML/Graphics.hpp>
#include "LevelGrid.h"
#include "GameObject.h"
#include "utilities.h"

class Player : public GameObject {
public:
    Player(int startX, int startY, float tileSize);
    void handleInput();
    void update(sf::Time dt);
    void draw(sf::RenderWindow& window) const override;
    void setWindowSize(sf::Vector2f size);
    void setGrid(LevelGrid* grid);
    sf::FloatRect getBounds() const override;
    sf::Vector2f getPosition() const;
    bool getIsDrawingPath() const { return isDrawingPath; }
    void setIsDrawingPath(bool val) { isDrawingPath = val; }
    void resetToStart();
    sf::Vector2i getDirection() const;

private:
    TileType getCurrentTile() const;
    sf::Vector2f previousPos;
    sf::Vector2f startPos;
    bool isDrawingPath;
    sf::Vector2f actualPos;
    sf::RectangleShape shape;
    float tileSize;
    float moveSpeed;
    sf::Vector2i gridDirection;
    sf::Vector2f windowSize;
    LevelGrid* gridRef = nullptr;
};
