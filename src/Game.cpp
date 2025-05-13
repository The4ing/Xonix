#include "Game.h"
#include "Player.h"
#include "Wall.h"

Game::Game()
    : window(sf::VideoMode(1920, 1080), "Xonix Game"),
    currentLevelNumber(0),
    lives(3),
    closedAreaPercent(0.0f),
    player(0, 29, 32.0f), //starting position
    grid(30, 60, 32.0f),
    hud(sf::Vector2f(0, 960), sf::Vector2f(1920, 120), font)
{
    font.loadFromFile("resources/arial.ttf"); // make sure arial.ttf is in your project folder

    player.setWindowSize(sf::Vector2f(window.getSize()));
    player.setGrid(&grid);

    gameObjects.push_back(&player);

    for (int row = 0; row < grid.getRows(); ++row) {
        for (int col = 0; col < grid.getCols(); ++col) {
            if (grid.get(row, col) == TileType::Wall) {
                sf::Vector2f pos(col * grid.getTileSize(), row * grid.getTileSize());
                walls.emplace_back(pos, grid.getTileSize());
            }
        }
    }

    for (Wall& wall : walls) {
        gameObjects.push_back(&wall);
    }
    enemies.emplace_back(sf::Vector2f(400, 300), 100.f, grid.getTileSize());
    gameObjects.push_back(&enemies.back());
    // place SmartEnemy at grid tile (10, 5)
    float tileSize = grid.getTileSize();
    sf::Vector2f smartEnemyPos(0 * tileSize, 0 * tileSize);
    smartEnemies.emplace_back(smartEnemyPos, tileSize);
    gameObjects.push_back(&smartEnemies.back());





}


void Game::run() {
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;

    while (window.isOpen()) {
        processEvents();
        timeSinceLastUpdate += clock.restart();

        while (timeSinceLastUpdate > TimePerFrame) {
            timeSinceLastUpdate -= TimePerFrame;
            processEvents();
            update(TimePerFrame);
        }

        render();
    }
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
    }
}

void Game::update(sf::Time dt) {
    player.handleInput();
    player.update(dt);
    for (auto& enemy : enemies)
        enemy.update(dt, grid);


    float elapsed = gameClock.getElapsedTime().asSeconds();
    hud.setTime(elapsed);
    hud.setScore(score);
    hud.setLives(lives);

    for (size_t i = 0; i < gameObjects.size(); ++i) {
        for (size_t j = i + 1; j < gameObjects.size(); ++j) {
            if (gameObjects[i]->getBounds().intersects(gameObjects[j]->getBounds())) {
                gameObjects[i]->collideWith(*gameObjects[j]);
                gameObjects[j]->collideWith(*gameObjects[i]);
            }
        }
    }
    for (auto& e : smartEnemies)
        e.update(dt, grid, player);

}

void Game::render() {
    window.clear();

    grid.draw(window);

    for (GameObject* obj : gameObjects) {
        if (auto* wall = dynamic_cast<Wall*>(obj)) {
            wall->draw(window);
        }
    }

    player.draw(window);
    for (const auto& enemy : enemies)
        enemy.draw(window);
    for (auto& e : smartEnemies)
        e.draw(window);


    hud.draw(window);
    window.display();
}
