#include "LevelGrid.h"

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
                tileShape.setFillColor(sf::Color::Red); // Grey for walls
                break;
            case TileType::Filled:
                tileShape.setFillColor(sf::Color::Red); // Green for filled area
                break;
            case TileType::Open:
                tileShape.setFillColor(sf::Color::Black); // Blue for open areas
                break;
            case TileType::PlayerPath:
                tileShape.setFillColor(sf::Color::Magenta); // Magenta for player path
                break;
            }

            // Set position for each tile
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
