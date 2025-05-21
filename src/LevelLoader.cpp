#include "LevelLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>

LevelLoader::LevelLoader(const std::string& filename) : filename(filename) {}

bool LevelLoader::load() {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Cannot open file " << filename << std::endl;
        return false;
    }

    std::string line;
    if (!std::getline(file, line)) {
        std::cerr << "Cannot read first line (settings)" << std::endl;
        return false;
    }

  //  std::cout << "Settings line: " << line << std::endl;

    std::istringstream settingsStream(line);
    unsigned int w, h;
    if (!(settingsStream >> w >> h >> settings.initialLives)) {
        std::cerr << "Failed to parse settings line" << std::endl;
        return false;
    }
    settings.windowSize = sf::Vector2u(w, h);

    while (std::getline(file, line)) {
        std::istringstream ss(line);
        LevelData level;
        int enemyCount;
        if (!(ss >> level.requiredAreaPercent >> enemyCount)) {
            std::cerr << "Failed to parse level data line: " << line << std::endl;
            return false;
        }
        for (int i = 0; i < enemyCount; ++i) {
            float x, y;
            if (!(ss >> x >> y)) {
                std::cerr << "Failed to parse enemy positions" << std::endl;
                return false;
            }
            level.enemyPositions.emplace_back(x, y);
        }
        levels.push_back(level);
    }

    std::cout << "Levels loaded: " << levels.size() << std::endl;
    return true;
}

const GameSettings& LevelLoader::getSettings() const {
    return settings;
}

void LevelLoader::loadLevel(int index, LevelGrid& grid, std::vector<Enemy>& enemies, std::vector<SmartEnemy>& smartEnemies) {
    if (index < 0 || index >= levels.size()) return;

    enemies.clear();
    smartEnemies.clear();

    // אפס את הגריד ל-Open במקום Empty
    for (int row = 0; row < grid.getRows(); ++row) {
        for (int col = 0; col < grid.getCols(); ++col) {
            grid.set(row, col, TileType::Open);
        }
    }

    // הוסף קירות מסביב
    for (int row = 0; row < grid.getRows(); ++row) {
        grid.set(row, 0, TileType::Wall);
        grid.set(row, grid.getCols() - 1, TileType::Wall);
    }
    for (int col = 0; col < grid.getCols(); ++col) {
        grid.set(0, col, TileType::Wall);
        grid.set(grid.getRows() - 1, col, TileType::Wall);
    }

    // טען אויבים רגילים
    for (const auto& pos : levels[index].enemyPositions) {
        enemies.emplace_back(pos, 100.f, grid.getTileSize());
    }

    // אויב חכם אחד
    smartEnemies.emplace_back(sf::Vector2f(0, 0), grid.getTileSize());
}

int LevelLoader::getLevelCount() const {
    return static_cast<int>(levels.size());
}

