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
    this->startPos = startPos;
}

void SmartEnemy::update(sf::Time dt, const LevelGrid& grid, const Player& player) {
    if (pathClock.getElapsedTime().asSeconds() >= 1.f) {
        findPath(grid, player);
        pathClock.restart();
    }

    moveAlongPath(dt);
}

void SmartEnemy::moveAlongPath(sf::Time dt) {
    if (path.empty()) return;

    sf::Vector2f currentPos = shape.getPosition();
    sf::Vector2i targetTile = path.front();

    sf::Vector2f targetPos = {
        targetTile.x * tileSize + tileSize / 2.f,
        targetTile.y * tileSize + tileSize / 2.f
    };

    sf::Vector2f direction = targetPos - currentPos;
    float dist = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (dist < moveSpeed * dt.asSeconds()) {
        shape.setPosition(targetPos);
        path.pop();
    }
    else {
        direction /= dist; // normalize
        shape.move(direction * moveSpeed * dt.asSeconds());
    }
}

void SmartEnemy::findPath(const LevelGrid& grid, const Player& player) {
    path = std::queue<sf::Vector2i>(); // clear path

    int rows = grid.getRows();
    int cols = grid.getCols();

    auto toGrid = [this](sf::Vector2f pos) -> sf::Vector2i {
        return sf::Vector2i(
            static_cast<int>(pos.x / tileSize),
            static_cast<int>(pos.y / tileSize)
        );
        };

    sf::Vector2i start = toGrid(shape.getPosition());
    sf::Vector2i goal = toGrid(player.getPosition());

    std::queue<sf::Vector2i> q;
    std::unordered_map<sf::Vector2i, sf::Vector2i> cameFrom;
    std::unordered_set<sf::Vector2i> visited;

    q.push(start);
    visited.insert(start);

    const std::vector<sf::Vector2i> directions = {
        {1,0}, {-1,0}, {0,1}, {0,-1}
    };

    while (!q.empty()) {
        sf::Vector2i curr = q.front(); q.pop();

        if (curr == goal) {
            break;
        }

        for (auto dir : directions) {
            sf::Vector2i next = curr + dir;

            if (next.x < 0 || next.x >= cols || next.y < 0 || next.y >= rows)
                continue;

            if (visited.count(next)) continue;

            TileType tile = grid.get(next.y, next.x);
            if (tile != TileType::Wall && tile != TileType::Filled)
                continue;

            q.push(next);
            visited.insert(next);
            cameFrom[next] = curr;
        }
    }

    // Backtrack path
    if (!cameFrom.count(goal)) return; // no path found

    sf::Vector2i current = goal;
    std::vector<sf::Vector2i> revPath;

    while (current != start) {
        revPath.push_back(current);
        current = cameFrom[current];
    }

    std::reverse(revPath.begin(), revPath.end());

    for (auto& step : revPath)
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
  //  std::cout << "Smart enemy hit player!\n";
}

sf::Vector2f SmartEnemy::getPosition() const {
    return shape.getPosition();
}

void SmartEnemy::resetToStart() {
    shape.setPosition(startPos);
    path = {};
}