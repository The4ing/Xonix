
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
                tileShape.setFillColor(sf::Color(100, 100, 100));
                break;
            case TileType::Filled:
                tileShape.setFillColor(sf::Color(100, 100, 100));
                break;
            case TileType::Open:
                tileShape.setFillColor(sf::Color::Black);
                break;
            case TileType::PlayerPath:
                tileShape.setFillColor(sf::Color(100,100,100));
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
    std::unordered_set<sf::Vector2i> enemyTiles;
    for (const auto& e : enemyPositions) {
        enemyTiles.insert({ static_cast<int>(e.x / tileSize), static_cast<int>(e.y / tileSize) });
    }

    std::unordered_set<sf::Vector2i> visited;
    std::vector<std::unordered_set<sf::Vector2i>> enclosedAreas;
    const std::vector<sf::Vector2i> directions = { {1,0}, {-1,0}, {0,1}, {0,-1} };

    auto inBounds = [&](sf::Vector2i p) {
        return p.x >= 0 && p.x < cols && p.y >= 0 && p.y < rows;
        };

    auto bfs = [&](sf::Vector2i start) {
        std::queue<sf::Vector2i> q;
        std::unordered_set<sf::Vector2i> area;
        bool touchesBorder = false;

        q.push(start);
        visited.insert(start);
        area.insert(start);

        while (!q.empty()) {
            auto curr = q.front(); q.pop();
            if (curr.x == 0 || curr.x == cols - 1 || curr.y == 0 || curr.y == rows - 1)
                touchesBorder = true;

            for (const auto& d : directions) {
                sf::Vector2i next = curr + d;
                if (!inBounds(next) || visited.count(next) || get(next.y, next.x) != TileType::Open) continue;
                q.push(next);
                visited.insert(next);
                area.insert(next);
            }
        }

        if (!touchesBorder) enclosedAreas.push_back(area);
        };

    // Scan grid for Open areas and run BFS
    for (int y = 0; y < rows; ++y)
        for (int x = 0; x < cols; ++x)
            if (get(y, x) == TileType::Open && !visited.count({ x, y }))
                bfs({ x, y });

    bool filled = false;
    for (const auto& area : enclosedAreas) {
        bool hasEnemy = std::any_of(area.begin(), area.end(), [&](const sf::Vector2i& pos) {
            return enemyTiles.count(pos);
            });

        if (!hasEnemy) {
            for (const auto& pos : area)
                set(pos.y, pos.x, TileType::Filled);
            filled = true;
        }
    }

    if (!filled)
        std::cout << "No area was filled (enemies in all areas). Only trail will be filled.\n";

    int pathConverted = 0;
    for (int y = 0; y < rows; ++y)
        for (int x = 0; x < cols; ++x)
            if (get(y, x) == TileType::PlayerPath) {
                set(y, x, TileType::Filled);
                ++pathConverted;
            }

    std::cout << "Converted PlayerPath to Filled: " << pathConverted << " tiles\n";
}
