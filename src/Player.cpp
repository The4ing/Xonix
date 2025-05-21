#include "Player.h"
#include <iostream>
#include "Player.h"
#include "Enemy.h"

using namespace std;

Player::Player(int startX, int startY, float tileSize)
    : tileSize(tileSize),
    moveSpeed(250.0f),
    actualPos(startX* tileSize, startY* tileSize),
    gridDirection(0, 0),
    isDrawingPath(false)
{
    shape.setSize(sf::Vector2f(tileSize, tileSize));
    shape.setFillColor(sf::Color::Green);
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
            std::cout << "[Player::update] setting to true" << std::endl;
            isDrawingPath = true;
        }

        // צבע את הריבוע של השחקן כתא נתיב
        int row = static_cast<int>((actualPos.y + tileSize / 2) / tileSize);
        int col = static_cast<int>((actualPos.x + tileSize / 2) / tileSize);
        if (gridRef->get(row, col) == TileType::Open) {
            gridRef->set(row, col, TileType::PlayerPath);
        }

    }
    //else if (currentTile == TileType::Wall && isDrawingPath) {
    //    std::cout << "[Player::update] setting to false" << std::endl;
    //    isDrawingPath = false;
    //    trailRects.clear();  // נקה את הנקודות הגרפיות (לא את הרשת)
    //}

    actualPos = nextPos;
    shape.setPosition(actualPos);

}


void Player::draw(sf::RenderWindow& window) const {
    for (const auto& dot : trailRects)
        window.draw(dot);

    window.draw(shape);
}

void Player::stop() {
    gridDirection = { 0, 0 };
}

sf::FloatRect Player::getBounds() const {
    return shape.getGlobalBounds();
}

void Player::collideWith(GameObject& other) {
    other.collideWithPlayer(*this);
}

void Player::collideWithEnemy(Enemy& enemy) {
   // std::cout << "Player collided with Enemy!" << std::endl;
}

void Player::collideWithWall(Wall& wall) {
    // Optional: stop();
}

void Player::collideWithSmartEnemy(SmartEnemy& smartEnemy) {
    // Handle collision logic
}



TileType Player::getCurrentTile() const {
    if (!gridRef) return TileType::Open;
    int row = static_cast<int>((actualPos.y + tileSize / 2) / tileSize);
    int col = static_cast<int>((actualPos.x + tileSize / 2) / tileSize);
    return gridRef->get(row, col);
}

std::string Player::tileTypeToString(TileType type) {
    switch (type) {
    case TileType::Empty:       return "Empty";
    case TileType::Wall:        return "Wall";
    case TileType::Filled:      return "Filled";
    case TileType::Open:        return "Open";
    case TileType::PlayerPath:  return "PlayerPath";
    default:                    return "Unknown";
    }
}


sf::Vector2f Player::getPosition() const {
    return actualPos;
}

void Player::resetToStart() {
    std::cout << "[Player::resetToStart] isDrawingPath was " << std::boolalpha << isDrawingPath << " -> false" << std::endl;
    actualPos = startPos;
    shape.setPosition(actualPos);
    gridDirection = { 0, 0 };
    isDrawingPath = false;
    trailRects.clear();
}

sf::Vector2i Player::getDirection() const {
    return gridDirection;
}

float Player::getSpeed() const {
    return moveSpeed;
}
