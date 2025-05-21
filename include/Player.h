
#pragma once
#include <SFML/Graphics.hpp>
#include "LevelGrid.h"
#include "GameObject.h"
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

    void collideWith(GameObject& other) override;
    void collideWithWall(Wall& wall) override;
    void collideWithEnemy(class Enemy& enemy) override;
    void collideWithSmartEnemy(class SmartEnemy& smartEnemy) override;

    sf::FloatRect getBounds() const override;
    sf::Vector2f getPosition() const;

    bool getIsDrawingPath() const { return isDrawingPath; }
    void setIsDrawingPath(bool val) { isDrawingPath = val; }

    void resetToStart();
    sf::Vector2i getDirection() const;
    float getSpeed() const;

private:

    sf::Vector2f previousPos;

    TileType getCurrentTile() const;
    std::string tileTypeToString(TileType type);
    sf::Vector2f startPos;
    std::vector<sf::RectangleShape> trailRects;

    bool isDrawingPath;
    sf::Vector2f actualPos;
    sf::RectangleShape shape;
    float tileSize;
    float moveSpeed;
    sf::Vector2i gridDirection;
    sf::Vector2f windowSize;
    LevelGrid* gridRef = nullptr;
};
