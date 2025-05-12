#include "Game.h"
#include "Player.h"
#include "Wall.h"

Game::Game()
    : window(sf::VideoMode(1920, 1080), "Xonix Game"),
    currentLevelNumber(0),
    lives(3),
    closedAreaPercent(0.0f),
    player(5, 5, 32.0f),
    grid(30, 60, 32.0f)
{
    player.setWindowSize(sf::Vector2f(window.getSize()));
    player.setGrid(&grid);

    gameObjects.push_back(&player);

    // ?? Create Wall objects from the grid
    for (int row = 0; row < grid.getRows(); ++row) {
        for (int col = 0; col < grid.getCols(); ++col) {
            if (grid.get(row, col) == TileType::Wall) {
                sf::Vector2f pos(col * grid.getTileSize(), row * grid.getTileSize());
                walls.emplace_back(pos, grid.getTileSize());
            }
        }
    }

    // ?? Add wall pointers to gameObjects
    for (Wall& wall : walls) {
        gameObjects.push_back(&wall);
    }
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

    // ?? Collision loop (double dispatch)
    for (size_t i = 0; i < gameObjects.size(); ++i) {
        for (size_t j = i + 1; j < gameObjects.size(); ++j) {
            if (gameObjects[i]->getBounds().intersects(gameObjects[j]->getBounds())) {
                gameObjects[i]->collideWith(*gameObjects[j]);
                gameObjects[j]->collideWith(*gameObjects[i]);
            }
        }
    }
}

void Game::render() {
    window.clear();

    grid.draw(window);

    // draw all walls (from gameObjects)
    for (GameObject* obj : gameObjects) {
        if (auto* wall = dynamic_cast<Wall*>(obj)) {
            wall->draw(window);
        }
    }

    player.draw(window);

    window.display();
}
