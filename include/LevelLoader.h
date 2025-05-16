#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "LevelGrid.h"
#include "Enemy.h"
#include "SmartEnemy.h"

struct GameSettings {
    sf::Vector2u windowSize;
    int initialLives;
};

struct LevelData {
    float requiredAreaPercent;
    std::vector<sf::Vector2f> enemyPositions;
};

class LevelLoader {
public:
    LevelLoader(const std::string& filename);
    int getLevelCount() const;
    bool load();

    const GameSettings& getSettings() const;
    void loadLevel(int index, LevelGrid& grid, std::vector<Enemy>& enemies, std::vector<SmartEnemy>& smartEnemies);

private:
    std::string filename;
    GameSettings settings;
    std::vector<LevelData> levels;
};
