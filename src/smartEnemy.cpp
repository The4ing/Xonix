#include "SmartEnemy.h"
#include "Player.h"
#include <cmath>
#include <unordered_set>
#include <iostream>

namespace std {
    template<>
    struct hash<sf::Vector2i> {
        std::size_t operator()(const sf::Vector2i& v) const noexcept {
            return std::hash<int>()(v.x) ^ (std::hash<int>()(v.y) << 1);
        }
    };
}


SmartEnemy::SmartEnemy(const sf::Vector2f& startPos, float tileSize)
    : tileSize(tileSize)
{
    shape.setRadius(tileSize / 2.f);
    shape.setOrigin(tileSize / 2.f, tileSize / 2.f);
    shape.setFillColor(sf::Color::Yellow);
    shape.setPosition(startPos);
}

void SmartEnemy::update(sf::Time dt, const LevelGrid& grid, const Player& player) {
    if (pathClock.getElapsedTime().asSeconds() >= 1.f) {
        findPath(grid, player);
        pathClock.restart();
    }

    moveAlongPath(dt, grid);
    std::cout << "Path length: " << path.size() << std::endl;

}

void SmartEnemy::moveAlongPath(sf::Time dt, const LevelGrid& grid) {
    if (path.empty()) return;

    sf::Vector2i targetTile = path.front();
    TileType tile = grid.get(targetTile.y, targetTile.x);
    std::cout << "SmartEnemy trying to move to tile (" << targetTile.x << ", " << targetTile.y << ") - TileType: ";

    switch (tile) {
    case TileType::Wall: std::cout << "Wall"; break;
    case TileType::Filled: std::cout << "Filled"; break;
    case TileType::Open: std::cout << "Open"; break;
    case TileType::PlayerPath: std::cout << "PlayerPath"; break;
    }
    std::cout << std::endl;


    if (tile != TileType::Wall && tile != TileType::Filled) {
        // ? Stop if the next step is illegal
        return;
    }

    sf::Vector2f currentPos = shape.getPosition();
    sf::Vector2f targetPos = {
        targetTile.x * tileSize + tileSize / 2.f,
        targetTile.y * tileSize + tileSize / 2.f
    };

    sf::Vector2f direction = targetPos - currentPos;
    float dist = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (dist < moveSpeed * dt.asSeconds()) {
        shape.setPosition(targetPos);
        path.pop(); // Step taken
    }
    else {
        direction /= dist;
        shape.move(direction * moveSpeed * dt.asSeconds());
    }
    sf::Vector2i currentTile = {
    static_cast<int>(shape.getPosition().x / tileSize),
    static_cast<int>(shape.getPosition().y / tileSize)
    };
    std::cout << "SmartEnemy currently at tile (" << currentTile.x << ", " << currentTile.y << ")" << std::endl;

}



void SmartEnemy::findPath(const LevelGrid& grid, const Player& player) {
    path = std::queue<sf::Vector2i>();

    int rows = grid.getRows();
    int cols = grid.getCols();

    auto toGrid = [this](sf::Vector2f pos) -> sf::Vector2i {
        return {
            static_cast<int>(pos.x / tileSize),
            static_cast<int>(pos.y / tileSize)
        };
        };

    sf::Vector2i start = toGrid(shape.getPosition());
    sf::Vector2i goal = toGrid(player.getPosition());

    std::queue<sf::Vector2i> q;
    std::unordered_map<sf::Vector2i, sf::Vector2i> cameFrom;
    std::unordered_set<sf::Vector2i> visited;

    q.push(start);
    visited.insert(start);

    const std::vector<sf::Vector2i> directions = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1}
    };

    while (!q.empty()) {
        sf::Vector2i curr = q.front(); q.pop();

        if (curr == goal) break;

        for (auto dir : directions) {
            sf::Vector2i next = curr + dir;

            if (next.x < 0 || next.x >= cols || next.y < 0 || next.y >= rows)
                continue;

            if (visited.count(next)) continue;

            // ? No tile filtering here!
            visited.insert(next);
            cameFrom[next] = curr;
            q.push(next);
        }
    }

    if (!cameFrom.count(goal)) return; // No path found

    std::vector<sf::Vector2i> revPath;
    for (sf::Vector2i at = goal; at != start; at = cameFrom[at])
        revPath.push_back(at);

    std::reverse(revPath.begin(), revPath.end());

    for (const auto& step : revPath)
        path.push(step);
}


void SmartEnemy::draw(sf::RenderWindow& window) const {
    window.draw(shape);
}

sf::FloatRect SmartEnemy::getBounds() const {
    return shape.getGlobalBounds();
}

void SmartEnemy::collideWith(GameObject& other) {
    other.collideWithSmartEnemy(*this);  
}


void SmartEnemy::collideWithPlayer(Player& player) {
    std::cout << "Smart enemy hit player!\n";
}

