
#include "LevelGrid.h"
#include <iostream>
#include <unordered_set>
#include <queue>

namespace std {
    template <>
    struct hash<sf::Vector2i> {
        std::size_t operator()(const sf::Vector2i& v) const noexcept {
            return std::hash<int>()(v.x) ^ (std::hash<int>()(v.y) << 1);
        }
    };
}

LevelGrid::LevelGrid(int r, int c, float tSize)
    : rows(r), cols(c), tileSize(tSize) {
    grid.resize(rows, std::vector<TileType>(cols, TileType::Open));

    for (int y = 0; y < rows; ++y) {
        grid[y][0] = TileType::Wall;
        grid[y][cols - 1] = TileType::Wall;
    }
    for (int x = 0; x < cols; ++x) {
        grid[0][x] = TileType::Wall;
        grid[rows - 1][x] = TileType::Wall;
    }

    tileShape.setSize(sf::Vector2f(tileSize, tileSize));
}

void LevelGrid::draw(sf::RenderWindow& window) {
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            switch (grid[y][x]) {
            case TileType::Wall:
                tileShape.setFillColor(sf::Color(100, 100, 100)); // BLUE
                break;
            case TileType::Filled:
                tileShape.setFillColor(sf::Color::Blue); // Cyan
                break;
            case TileType::Open:
                tileShape.setFillColor(sf::Color::Black); // black
                break;
            case TileType::PlayerPath:
                tileShape.setFillColor(sf::Color::Magenta); // MAGENTA 
                break;
            }
            tileShape.setPosition(x * tileSize, y * tileSize);
            window.draw(tileShape);
        }
    }
}


TileType LevelGrid::get(int row, int col) const {
    if (row < 0 || row >= rows || col < 0 || col >= cols)
        return TileType::Wall;
    return grid[row][col];
}

void LevelGrid::set(int row, int col, TileType type) {
    if (row < 0 || row >= rows || col < 0 || col >= cols)
        return;
    grid[row][col] = type;
}

void LevelGrid::fillEnclosedArea(const std::vector<sf::Vector2f>& enemyPositions) {
    std::vector<std::unordered_set<sf::Vector2i>> enclosedAreas;
    std::unordered_set<sf::Vector2i> visited;

    const std::vector<sf::Vector2i> directions = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1}
    };

    std::unordered_set<sf::Vector2i> enemyTiles;
    for (const auto& e : enemyPositions) {
        int ex = static_cast<int>(e.x / tileSize);
        int ey = static_cast<int>(e.y / tileSize);
        enemyTiles.insert({ ex, ey });
    }

    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            sf::Vector2i start(x, y);
            if (get(y, x) != TileType::Open || visited.count(start)) continue;

            std::unordered_set<sf::Vector2i> area;
            std::queue<sf::Vector2i> q;
            bool touchesBorder = false;

            q.push(start);
            visited.insert(start);
            area.insert(start);

            while (!q.empty()) {
                sf::Vector2i curr = q.front(); q.pop();

                if (curr.x == 0 || curr.x == cols - 1 || curr.y == 0 || curr.y == rows - 1)
                    touchesBorder = true;

                for (auto dir : directions) {
                    sf::Vector2i next = curr + dir;
                    if (next.x < 0 || next.x >= cols || next.y < 0 || next.y >= rows)
                        continue;

                    if (get(next.y, next.x) == TileType::Open && !visited.count(next)) {
                        visited.insert(next);
                        q.push(next);
                        area.insert(next);
                    }
                }
            }

            if (!touchesBorder) {
                enclosedAreas.push_back(area);
            }
        }
    }

    int filledTiles = 0;
    for (const auto& area : enclosedAreas) {
        bool hasEnemy = false;
        for (const auto& tile : area) {
            if (enemyTiles.count(tile)) {
                hasEnemy = true;
                break;
            }
        }

        if (!hasEnemy) {
            for (const auto& tile : area) {
                set(tile.y, tile.x, TileType::Filled);
                filledTiles++;
            }
        }
    }

    int trailConverted = 0;
    for (int y = 0; y < rows; ++y)
        for (int x = 0; x < cols; ++x)
            if (get(y, x) == TileType::PlayerPath) {
                set(y, x, TileType::Filled);
                trailConverted++;
            }
}


float LevelGrid::calculateClosedAreaPercent() const {
    int closedCount = 0;
    int totalCount = rows * cols;

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (grid[r][c] == TileType::PlayerPath) {
                closedCount++;
            }
        }
    }
    if (totalCount == 0)
        return 0.0f;

    return (static_cast<float>(closedCount) / totalCount) * 100.0f;
}

bool LevelGrid::isCollidable(int row, int col) const {
    TileType t = get(row, col);
    return t == TileType::Wall || t == TileType::Filled;
}
