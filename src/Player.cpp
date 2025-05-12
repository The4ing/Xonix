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

    // Define edges of player at next position
    float left = nextPos.x;
    float right = nextPos.x + tileSize;
    float top = nextPos.y;
    float bottom = nextPos.y + tileSize;

    // Get window boundaries
    float maxX = windowSize.x;
    float maxY = HUD_TOP_Y ;


    // If next position is out of window bounds — stop movement
    if (left < 0 || right > maxX || top < 0 || bottom > maxY) {
        return;
    }

    // Allow movement on any tile, including Wall and Filled
    actualPos = nextPos;
    shape.setPosition(actualPos);
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
    //stop(); // or something more sophisticated
}
