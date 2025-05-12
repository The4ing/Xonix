#include "Player.h"
#include "Enemy.h" // forward declared in GameObject


Player::Player(int startX, int startY, float tileSize)
    : tileSize(tileSize),
    moveSpeed(250.0f),
    actualPos(startX* tileSize, startY* tileSize),
    gridDirection(0, 0)
{
    shape.setSize(sf::Vector2f(tileSize, tileSize));
    shape.setFillColor(sf::Color::Green);
    shape.setPosition(actualPos);
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

    // Compute wall collision with pixel offset logic
    const float pixelOffset = 4.0f;

    float left = nextPos.x;
    float right = nextPos.x + tileSize;
    float top = nextPos.y;
    float bottom = nextPos.y + tileSize;

    int tileTop = static_cast<int>(top / tileSize);
    int tileBottom = static_cast<int>((bottom - 1) / tileSize);
    int tileLeft = static_cast<int>(left / tileSize);
    int tileRight = static_cast<int>((right - 1) / tileSize);

    bool blocked = false;

    if (gridDirection.y == -1) { // Up
        float bottomEdge = nextPos.y + tileSize;
        int wallRow = static_cast<int>(bottomEdge / tileSize);
        if ((gridRef->get(wallRow, tileLeft) == TileType::Wall ||
            gridRef->get(wallRow, tileRight) == TileType::Wall) &&
            bottomEdge > (wallRow + 1) * tileSize + pixelOffset) {
            blocked = true;
        }
    }
    else if (gridDirection.y == 1) { // Down
        float topEdge = nextPos.y;
        int wallRow = static_cast<int>(topEdge / tileSize);
        if ((gridRef->get(wallRow, tileLeft) == TileType::Wall ||
            gridRef->get(wallRow, tileRight) == TileType::Wall) &&
            topEdge < wallRow * tileSize - pixelOffset) {
            blocked = true;
        }
    }
    else if (gridDirection.x == -1) { // Left
        float rightEdge = nextPos.x + tileSize;
        int wallCol = static_cast<int>((rightEdge) / tileSize);
        if ((gridRef->get(tileTop, wallCol) == TileType::Wall ||
            gridRef->get(tileBottom, wallCol) == TileType::Wall) &&
            rightEdge > (wallCol + 1) * tileSize + pixelOffset) {
            blocked = true;
        }
    }
    else if (gridDirection.x == 1) { // Right
        float leftEdge = nextPos.x;
        int wallCol = static_cast<int>(leftEdge / tileSize);
        if ((gridRef->get(tileTop, wallCol) == TileType::Wall ||
            gridRef->get(tileBottom, wallCol) == TileType::Wall) &&
            leftEdge < wallCol * tileSize - pixelOffset) {
            blocked = true;
        }
    }

    if (!blocked) {
        actualPos = nextPos;
        shape.setPosition(actualPos);
    }
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

void Player::stop() {
    gridDirection = { 0, 0 };
}

sf::FloatRect Player::getBounds() const {
    return shape.getGlobalBounds();
}

// ?? Double dispatch handlers
void Player::collideWith(GameObject& other) {
    other.collideWithPlayer(*this);
}

//void Player::collideWithEnemy(Enemy& enemy) {
//    std::cout << "Player collided with Enemy!" << std::endl;
//}

void Player::collideWithWall(Wall& wall) {
    std::cout << "Player hit a wall (from Player side)" << std::endl;
    stop(); // or something more sophisticated
}
