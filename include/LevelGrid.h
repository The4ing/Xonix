
#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "TileType.h"
#include <unordered_set>
#include <queue>

class LevelGrid {
public:
    LevelGrid(int rows, int cols, float tileSize);
    void draw(sf::RenderWindow& window);
    TileType get(int row, int col) const;
    void set(int row, int col, TileType type);
    int getRows() const { return rows; }
    int getCols() const { return cols; }
    float getTileSize() const { return tileSize; }
    void fillEnclosedArea(const std::vector<sf::Vector2f>& enemyPositions);
    float calculateClosedAreaPercent() const;
    bool isCollidable(int row, int col) const;

private:
    int rows, cols;
    float tileSize;
    std::vector<std::vector<TileType>> grid;
    sf::RectangleShape tileShape;
};