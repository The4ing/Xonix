#include "Player.h"
#include "smartEnemy.h"

Player::Player(int startX, int startY, float tileSize)
    : tileSize(tileSize),
    moveSpeed(250.0f),
    actualPos(startX* tileSize, startY* tileSize),
    gridDirection(0, 0),
    isDrawingPath(false)
{
    shape.setSize(sf::Vector2f(tileSize, tileSize));
    shape.setFillColor(sf::Color::White);
    shape.setPosition(actualPos);
    startPos = actualPos;
}

void Player::setWindowSize(sf::Vector2f size) {
    windowSize = size;
}

void Player::setGrid(LevelGrid* grid) {
    gridRef = grid;
}

void Player::handleInput() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))    gridDirection = { 0, -1 };
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))  gridDirection = { 0, 1 };
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))  gridDirection = { -1, 0 };
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) gridDirection = { 1, 0 };
}

void Player::update(sf::Time dt) {
    if (!gridRef) return;

    sf::Vector2f velocity = sf::Vector2f(gridDirection) * moveSpeed * dt.asSeconds();
    sf::Vector2f nextPos = actualPos + velocity;

    float left = nextPos.x;
    float right = nextPos.x + tileSize;
    float top = nextPos.y;
    float bottom = nextPos.y + tileSize;

    float maxX = windowSize.x;
    float maxY = HUD_TOP_Y;

    if (left < 0 || right > maxX || top < 0 || bottom > maxY) {
        return;
    }

    TileType currentTile = getCurrentTile();

    if (currentTile == TileType::Open) {
        if (!isDrawingPath) {
            isDrawingPath = true;
        }

        // צבע את הריבוע של השחקן כתא נתיב
        int row = static_cast<int>((actualPos.y + tileSize / 2) / tileSize);
        int col = static_cast<int>((actualPos.x + tileSize / 2) / tileSize);
        if (gridRef->get(row, col) == TileType::Open) {
            gridRef->set(row, col, TileType::PlayerPath);
        }

    }

    actualPos = nextPos;
    shape.setPosition(actualPos);

}

void Player::draw(sf::RenderWindow& window) const {
    window.draw(shape);
}

sf::FloatRect Player::getBounds() const {
    return shape.getGlobalBounds();
}

TileType Player::getCurrentTile() const {
    if (!gridRef) return TileType::Open;
    int row = static_cast<int>((actualPos.y + tileSize / 2) / tileSize);
    int col = static_cast<int>((actualPos.x + tileSize / 2) / tileSize);
    return gridRef->get(row, col);
}

sf::Vector2f Player::getPosition() const {
    return actualPos;
}

void Player::resetToStart() {
    actualPos = startPos;
    shape.setPosition(actualPos);
    gridDirection = { 0, 0 };
    isDrawingPath = false;
}

sf::Vector2i Player::getDirection() const {
    return gridDirection;
}